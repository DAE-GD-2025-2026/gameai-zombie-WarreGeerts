#include "BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"
#include "DrawDebugHelpers.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "BTT Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp) return EBTNodeResult::Failed;

	APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn) return EBTNodeResult::Failed;

	UStudentPerceptorGeertsWarre* SP = Cast<UStudentPerceptorGeertsWarre>(
		OwnerPawn->GetComponentByClass(UStudentPerceptorGeertsWarre::StaticClass()));
	if (!SP) return EBTNodeResult::Failed;

	UInventoryComponent* TargetInventory = OwnerPawn->GetComponentByClass<UInventoryComponent>();
	if (!TargetInventory) return EBTNodeResult::Failed;

	AActor* TargetZombie = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetZombieKey.SelectedKeyName));
	if (TargetZombie && IsValid(TargetZombie))
	{
		FVector PawnLoc = OwnerPawn->GetActorLocation();
		FVector ZombieLoc = TargetZombie->GetActorLocation();

		FVector LookDir = (ZombieLoc - PawnLoc).GetSafeNormal();
		FRotator TargetRotation = LookDir.Rotation();
		TargetRotation.Pitch = 0.f;
		TargetRotation.Roll = 0.f;

		OwnerPawn->SetActorRotation(TargetRotation);

		UWorld* World = OwnerPawn->GetWorld();

		DrawDebugLine(World, PawnLoc, ZombieLoc, FColor::Orange, false, 1.f, 0, 3.f);

		DrawDebugSphere(World, ZombieLoc, 10.f, 12, FColor::Red, false, 1.f);

		DrawDebugSphere(World, PawnLoc, 10.f, 12, FColor::Yellow, false, 1.f);

		GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Orange,
		                                 FString::Printf(
			                                 TEXT("Attacking zombie at dist: %.1f"),
			                                 FVector::Dist(PawnLoc, ZombieLoc)));
	}

	const TArray<ABaseItem*>& InventoryItems = TargetInventory->GetInventory();

	for (int Index = 0; Index < InventoryItems.Num(); ++Index)
	{
		const ABaseItem* Item = InventoryItems[Index];
		if (!Item) continue;

		FString ItemClassName = Item->GetClass()->GetName();

		if (ItemClassName.Contains(TEXT("Shotgun")) || ItemClassName.Contains(TEXT("Pistol")))
		{
			TargetInventory->UseItem(Index);

			SP->GetTrackedZombies().Remove(nullptr);
			for (auto It = SP->GetTrackedZombies().CreateIterator(); It; ++It)
			{
				AActor* ZombieActor = *It;
				if (!IsValid(ZombieActor))
				{
					It.RemoveCurrent();
				}
			}

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
