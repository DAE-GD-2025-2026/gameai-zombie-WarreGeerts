#include "BTTask_FindFleeLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../StudentPerceptorGeertsWarre.h"

UBTTask_FindFleeLocation::UBTTask_FindFleeLocation()
{
	NodeName = "BTT Find Flee Location";
}

EBTNodeResult::Type UBTTask_FindFleeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComp) return EBTNodeResult::Failed;

	const APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn) return EBTNodeResult::Failed;

	UStudentPerceptorGeertsWarre* Perceptor = OwnerPawn->FindComponentByClass<UStudentPerceptorGeertsWarre>();
	if (!Perceptor) return EBTNodeResult::Failed;

	TSet<AActor*>& TrackedZombies = Perceptor->GetTrackedZombies();
	const FVector PlayerLocation = OwnerPawn->GetActorLocation();

	TArray<AActor*> ZombiesToRemove;
	for (AActor* Zombie : TrackedZombies)
	{
		if (!Zombie || !Zombie->IsValidLowLevel())
		{
			ZombiesToRemove.Add(Zombie);
			continue;
		}

		const float Distance = FVector::Dist(PlayerLocation, Zombie->GetActorLocation());
		if (Distance > MemoryRadius)
		{
			ZombiesToRemove.Add(Zombie);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
			                                 TEXT("Forgot zombie: Out of radius."));
		}
	}

	for (const AActor* ZombieToForget : ZombiesToRemove)
	{
		TrackedZombies.Remove(ZombieToForget);
	}
	
	if (TrackedZombies.Num() == 0)
	{
		BlackboardComp->SetValueAsBool(TEXT("HasTrackedZombies"), false);
		return EBTNodeResult::Failed;
	}

	if (TrackedZombies.Num() > 0)
	{
		FVector CombinedAwayDir = FVector::ZeroVector;

		for (const AActor* Zombie : TrackedZombies)
		{
			if (Zombie)
			{
				FVector ZombieLocation = Zombie->GetActorLocation();
				FVector AwayFromZombie = (PlayerLocation - ZombieLocation).GetSafeNormal();

				const float Distance = FVector::Dist(PlayerLocation, ZombieLocation);
				const float Weight = 1.0f / FMath::Max(Distance, 1.0f);

				CombinedAwayDir += AwayFromZombie * Weight;
			}
		}

		CombinedAwayDir = CombinedAwayDir.GetSafeNormal();
		const FVector FleeLocation = PlayerLocation + (CombinedAwayDir * 250.f);

		BlackboardComp->SetValueAsVector(FleeLocationKey.SelectedKeyName, FleeLocation);
		return EBTNodeResult::Succeeded;
	}

	BlackboardComp->ClearValue(FleeLocationKey.SelectedKeyName);
	return EBTNodeResult::Failed;
}
