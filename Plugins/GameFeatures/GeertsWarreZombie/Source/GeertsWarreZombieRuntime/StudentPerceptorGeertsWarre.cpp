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

		BlackboardComp->SetValueAsBool(TEXT("HasTrackedZombies"), TrackedZombies.Num() > 0);
	}
	else if (HouseCheck(Actor))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (!EnteredHouses.Contains(Actor))
			{
				EnteredHouses.Add(Actor);
             
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("House Registered: %s"), *Actor->GetName()));
			}
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
			BlackboardComp->SetValueAsObject(TEXT("TargetItem"), Actor);
			BlackboardComp->SetValueAsVector(TEXT("ItemLocation"), Actor->GetActorLocation());
		}
	}
}

void UStudentPerceptorGeertsWarre::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || Actor->GetName().Contains(TEXT("Garbage"))) return;
	UpdateBlackboardData(Actor, Stimulus);
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
