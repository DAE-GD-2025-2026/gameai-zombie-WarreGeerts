#include "BTTask_Charge.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"
#include "DrawDebugHelpers.h"

UBTTask_Charge::UBTTask_Charge()
{
	NodeName = "BTS Calculate Charge Location";

	Interval = 0.05f;
	RandomDeviation = 0.01f;
}

void UBTTask_Charge::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp) return;

	const APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn) return;

	UStudentPerceptorGeertsWarre* SP = Cast<UStudentPerceptorGeertsWarre>(
		OwnerPawn->GetComponentByClass(UStudentPerceptorGeertsWarre::StaticClass()));
	if (!SP) return;

	const TSet<AActor*>& Zombies = SP->GetTrackedZombies();
	if (Zombies.Num() == 0)
	{
		BlackboardComp->SetValueAsBool(TEXT("HasTrackedZombies"), false);
		BlackboardComp->SetValueAsBool(InRangeKey.SelectedKeyName, false);
		return;
	}

	AActor* NearestZombie = nullptr;
	float ClosestDistanceSq = FLT_MAX;
	FVector PawnLocation = OwnerPawn->GetActorLocation();

	for (AActor* Zombie : Zombies)
	{
		if (!Zombie || !IsValid(Zombie)) continue;
		float DistanceSq = FVector::DistSquared(PawnLocation, Zombie->GetActorLocation());
		if (DistanceSq < ClosestDistanceSq)
		{
			ClosestDistanceSq = DistanceSq;
			NearestZombie = Zombie;
		}
	}

	if (!NearestZombie)
	{
		BlackboardComp->SetValueAsBool(InRangeKey.SelectedKeyName, false);
		return;
	}

	BlackboardComp->SetValueAsObject(TargetZombieKey.SelectedKeyName, NearestZombie);

	FVector ZombieLocation = NearestZombie->GetActorLocation();
	float CurrentDistance = FVector::Dist(PawnLocation, ZombieLocation);
	FVector DirectionToZombie = (ZombieLocation - PawnLocation).GetSafeNormal();

	if (CurrentDistance > Distance)
	{
		FVector ChargeTargetLocation = ZombieLocation - (DirectionToZombie * Distance);
		BlackboardComp->SetValueAsVector(ChargeLocationKey.SelectedKeyName, ChargeTargetLocation);
		BlackboardComp->SetValueAsBool(InRangeKey.SelectedKeyName, false);
	}
	else
	{
		BlackboardComp->SetValueAsBool(InRangeKey.SelectedKeyName, true);
	}
	
	if (NearestZombie)
	{
		UWorld* World = OwnerPawn->GetWorld();
    
		DrawDebugSphere(World, NearestZombie->GetActorLocation(), 15.f, 12, FColor::Red, false, 0.1f);
    
		FVector DebugTarget = BlackboardComp->GetValueAsVector(ChargeLocationKey.SelectedKeyName);
		DrawDebugSphere(World, DebugTarget, 15.f, 12, FColor::Green, false, 0.1f);
    
		DrawDebugLine(World, PawnLocation, DebugTarget, FColor::Blue, false, 0.1f, 0, 2.f);
    
		DrawDebugLine(World, PawnLocation, NearestZombie->GetActorLocation(), FColor::White, false, 0.1f, 0, 1.f);

		float CurrentDist = FVector::Dist(PawnLocation, NearestZombie->GetActorLocation());
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Yellow,
			FString::Printf(TEXT("Zombie dist: %.1f | Stop dist: %.1f | InRange: %s"),
				CurrentDist, Distance,
				CurrentDist <= Distance ? TEXT("YES") : TEXT("NO")));
	}
}
