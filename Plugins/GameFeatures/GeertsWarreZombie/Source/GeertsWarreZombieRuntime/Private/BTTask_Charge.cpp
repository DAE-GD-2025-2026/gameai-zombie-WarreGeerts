#include "BTTask_Charge.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTTask_Charge::UBTTask_Charge()
{
	NodeName = "BTT Charge";
}

EBTNodeResult::Type UBTTask_Charge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
		                                 TEXT("Invalid AiController or BlackboardComp"));
		return EBTNodeResult::Failed;
	}

	const APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
		                                 TEXT("Invalid OwnerPawn"));
		return EBTNodeResult::Failed;
	}

	UStudentPerceptorGeertsWarre* SP =
		Cast<UStudentPerceptorGeertsWarre>(OwnerPawn->GetComponentByClass(UStudentPerceptorGeertsWarre::StaticClass()));
	if (!SP)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
		                                 TEXT("Invalid UStudentPerceptorGeertsWarre"));
		return EBTNodeResult::Failed;
	}

	const TSet<AActor*>& Zombies = SP->GetTrackedZombies();

	if (Zombies.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
			TEXT("No zombies in memory to charge!"));
		BlackboardComp->SetValueAsBool(TEXT("HasTrackedZombies"), false);
		return EBTNodeResult::Failed;
	}

	AActor* NearestZombie = nullptr;
	float ClosestDistanceSq = FLT_MAX; 
	FVector PawnLocation = OwnerPawn->GetActorLocation();

	for (AActor* Zombie : Zombies)
	{
		if (!Zombie) continue;

		float DistanceSq = FVector::DistSquared(PawnLocation, Zombie->GetActorLocation());
		if (DistanceSq < ClosestDistanceSq)
		{
			ClosestDistanceSq = DistanceSq;
			NearestZombie = Zombie;
		}
	}

	if (!NearestZombie) return EBTNodeResult::Failed;

	FVector ZombieLocation = NearestZombie->GetActorLocation();
	FVector DirectionToZombie = (ZombieLocation - PawnLocation).GetSafeNormal();

	float CurrentDistance = FVector::Dist(PawnLocation, ZombieLocation);
	FVector ChargeTargetLocation;
	if (CurrentDistance > Distance)
	{
		// 1. Zombie is too far away! Move closer, but stop exactly at weapon range.
		ChargeTargetLocation = ZombieLocation - (DirectionToZombie * Distance);
	}
	else
	{
		// 2. Zombie is already within weapon range! Stand completely still right here.
		ChargeTargetLocation = PawnLocation;
	}
	BlackboardComp->SetValueAsObject(TEXT("TargetZombie"), NearestZombie);
	BlackboardComp->SetValueAsVector(TEXT("ChargeLocation"), ChargeTargetLocation);

	return EBTNodeResult::Succeeded;
}
