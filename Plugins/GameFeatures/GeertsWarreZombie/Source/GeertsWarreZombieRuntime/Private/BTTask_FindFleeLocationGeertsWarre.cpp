#include "BTTask_FindFleeLocationGeertsWarre.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../StudentPerceptorGeertsWarre.h"

UBTTask_FindFleeLocationGeertsWarre::UBTTask_FindFleeLocationGeertsWarre()
{
	NodeName = "BTS Find Flee Location";
	Interval = 0.1f;
	RandomDeviation = 0.02f;
}

void UBTTask_FindFleeLocationGeertsWarre::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp) return;

	const APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn) return;

	UStudentPerceptorGeertsWarre* Perceptor = OwnerPawn->FindComponentByClass<UStudentPerceptorGeertsWarre>();
	if (!Perceptor) return;

	TSet<AActor*>& TrackedZombies = Perceptor->GetTrackedZombies();
	const FVector PlayerLocation = OwnerPawn->GetActorLocation();

	// ... Keep your exact zombie distance checks and removal logic here ...

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
		}
	}

	for (const AActor* ZombieToForget : ZombiesToRemove)
	{
		TrackedZombies.Remove(ZombieToForget);
	}
	
	if (TrackedZombies.Num() == 0)
	{
		BlackboardComp->SetValueAsBool(TEXT("HasTrackedZombies"), false);
		return; // Services don't return NodeResults
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
		return;
	}

	BlackboardComp->ClearValue(FleeLocationKey.SelectedKeyName);
}
