// Fill out your copyright notice in the Description page of Project Settings.

#include "StudentPerceptorGeertsWarre.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UStudentPerceptorGeertsWarre::UStudentPerceptorGeertsWarre()
{
	PrimaryComponentTick.bCanEverTick = true;
    
	Brain = CreateDefaultSubobject<UPlayerBrainGeertsWarre>(TEXT("PlayerBrain"));
	Inventory = CreateDefaultSubobject<UPlayerInventoryGeertsWarre>(TEXT("PlayerInventory"));	
}

void UStudentPerceptorGeertsWarre::BeginPlay()
{
	Super::BeginPlay();

	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptorGeertsWarre::OnPerceptionUpdated);
	}

	
}

void UStudentPerceptorGeertsWarre::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	if (!AIController) return;

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp) return;

	const FVector PlayerLocation = OwnerPawn->GetActorLocation();

	Brain->ForgetZombies(PlayerLocation);

	if (Brain->GetTrackedZombies().Num() > 0)
	{
		FVector CombinedAwayDir = FVector::ZeroVector;

		for (AActor* Zombie : Brain->GetTrackedZombies())
		{
			if (Zombie)
			{
				FVector ZombieLocation = Zombie->GetActorLocation();
				FVector AwayFromZombie = (PlayerLocation - ZombieLocation).GetSafeNormal();

				float Distance = FVector::Dist(PlayerLocation, ZombieLocation);
				float Weight = 1.0f / FMath::Max(Distance, 1.0f);

				CombinedAwayDir += AwayFromZombie * Weight;
			}
		}

		CombinedAwayDir = CombinedAwayDir.GetSafeNormal();
		FVector FleeLocation = PlayerLocation + (CombinedAwayDir * 100.f);

		BlackboardComp->SetValueAsVector(TEXT("FleeLocation"), FleeLocation);

		//Debug
		DrawDebugLine(GetWorld(), PlayerLocation, FleeLocation, FColor::Emerald,
		              false, -1.f, 0, 4.f);
	}
	else
	{
		FVector CurrentFlee = BlackboardComp->GetValueAsVector(TEXT("FleeLocation"));
		if (!CurrentFlee.IsZero())
		{
			BlackboardComp->ClearValue(TEXT("FleeLocation"));
		}
	}
}

void UStudentPerceptorGeertsWarre::UpdateBlackboardData(AActor* Actor, const FAIStimulus& Stimulus)
{
	if (Brain->ZombieCheck(Actor))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			Brain->GetTrackedZombies().Add(Actor);
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, 
					TEXT("Zombie spotted and added to memory!"));
		}
	}
	else
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (!OwnerPawn) return;

		AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
		if (!AIController) return;

		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (!BlackboardComp) return;

		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
			BlackboardComp->SetValueAsVector(TEXT("TargetLocation"), Actor->GetActorLocation());

			
			if (Inventory->PickUp(Actor))
			{
				BlackboardComp->ClearValue(TEXT("TargetActor"));
				BlackboardComp->ClearValue(TEXT("TargetLocation"));
			}
		}
		else
		{
			BlackboardComp->ClearValue(TEXT("TargetActor"));
			BlackboardComp->ClearValue(TEXT("TargetLocation"));
		}
	}
}

void UStudentPerceptorGeertsWarre::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || Actor->GetName().Contains(TEXT("Garbage"))) return;
	UpdateBlackboardData(Actor, Stimulus);
}





