#include "StudentPerceptorGeertsWarre.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"

UStudentPerceptorGeertsWarre::UStudentPerceptorGeertsWarre()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStudentPerceptorGeertsWarre::BeginPlay()
{
	Super::BeginPlay();

	if (const auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptorGeertsWarre::OnPerceptionUpdated);
	}
}

void UStudentPerceptorGeertsWarre::UpdateBlackboardData(AActor* Actor, const FAIStimulus& Stimulus)
{
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	if (!AIController) return;

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp) return;

	if (ZombieCheck(Actor))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			TrackedZombies.Add(Actor);
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange,
			                                 TEXT("Zombie spotted and added to memory!"));
		}

		TrackedZombies.Remove(nullptr);
       
		for (auto It = TrackedZombies.CreateIterator(); It; ++It)
		{
			AActor* ZombieActor = *It;
			if (!IsValid(ZombieActor))
			{
				It.RemoveCurrent();
			}
		}
		
		BlackboardComp->SetValueAsBool(TEXT("HasTrackedZombies"), TrackedZombies.Num() > 0);
	}
	else if (HouseCheck(Actor))
	{
		if (CheckedHouses.Contains(Actor)) return;
       
		if (Stimulus.WasSuccessfullySensed())
		{
			AActor* ActiveTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetHouse")));
			if (ActiveTarget != nullptr && ActiveTarget != Actor)
			{
				return;
			}

			House = Actor;
			BlackboardComp->SetValueAsObject(TEXT("TargetHouse"), Actor);
			BlackboardComp->SetValueAsVector(TEXT("HouseLocation"), Actor->GetActorLocation());
		}
		else
		{
			AActor* CurrentHouse = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetHouse")));
			if (CurrentHouse == Actor)
			{
				BlackboardComp->ClearValue(TEXT("TargetHouse"));
				BlackboardComp->ClearValue(TEXT("HouseLocation"));
			}
		}
	}

	else
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (GroundedItems.Contains(Actor)) return;

			GroundedItems.Add(Actor);

			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple,
			                                 FString::Printf(
				                                 TEXT("Item: %s, Location: %s")
				                                 , *Actor->GetName(), *Actor->GetActorLocation().ToCompactString()));
		}
	}
}

void UStudentPerceptorGeertsWarre::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || Actor->GetName().Contains(TEXT("Garbage"))) return;
	UpdateBlackboardData(Actor, Stimulus);
}

void UStudentPerceptorGeertsWarre::MarkCurrentHouseAsChecked()
{
	if (House != nullptr)
	{
		CheckedHouses.Add(House);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green,
										 FString::Printf(
											 TEXT("Added house to checked houses")));
		House = nullptr;
	}
}

bool UStudentPerceptorGeertsWarre::ZombieCheck(AActor* Actor)
{
	if (const APawn* Pawn = Cast<APawn>(Actor))
	{
		if (const AController* Controller = Pawn->GetController())
		{
			if (Controller->GetClass()->GetName().Contains(TEXT("BP_ZombieController")))
			{
				return true;
			}
		}
	}
	return false;
}

bool UStudentPerceptorGeertsWarre::HouseCheck(AActor* Actor)
{
	if (!Actor) return false;

	return Actor->GetName().Contains(TEXT("House")) ||
		Actor->GetClass()->GetName().Contains(TEXT("House"));
}
