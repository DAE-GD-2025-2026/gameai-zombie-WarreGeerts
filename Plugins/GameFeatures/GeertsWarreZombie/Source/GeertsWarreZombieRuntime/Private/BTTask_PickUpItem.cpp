#include "BTTask_PickUpItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"
#include "Engine/Engine.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"


UBTTask_PickUpItem::UBTTask_PickUpItem()
{
	NodeName = "BTT Pickup Item";
}

EBTNodeResult::Type UBTTask_PickUpItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow,
	                                 TEXT("Picking Up Item. . ."));

	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
		                                 TEXT("Invalid AIController or Blackboard"));
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

	AActor* ItemActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetItemKey.SelectedKeyName));
	if (!ItemActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
		                                 TEXT("Invalid ItemActor"));
		BlackboardComp->ClearValue(TargetItemKey.SelectedKeyName);
		return EBTNodeResult::Failed;
	}

	UInventoryComponent* TargetInventory = OwnerPawn->GetComponentByClass<UInventoryComponent>();
	if (!TargetInventory) return EBTNodeResult::Failed;

	PickupRange = TargetInventory->GetPickupRange();

	const float DistanceToTarget = FVector::Dist(OwnerPawn->GetActorLocation(), ItemActor->GetActorLocation());
	if (DistanceToTarget > PickupRange)
	{
		return EBTNodeResult::Failed;
	}

	ABaseItem* PerceivedItem = Cast<ABaseItem>(ItemActor);
	if (!PerceivedItem) return EBTNodeResult::Failed;
	

	const TArray<ABaseItem*>& InventoryItems = TargetInventory->GetInventory();
	int TargetSlot = -1;

	for (int i = 0; i < InventoryItems.Num(); i++)
	{
		if (InventoryItems[i] == nullptr)
		{
			TargetSlot = i;
			break;
		}
	}

	if (TargetSlot != -1)
	{
		const bool bGrabSuccessful = TargetInventory->GrabItem(TargetSlot, PerceivedItem);
		if (!bGrabSuccessful) return EBTNodeResult::Failed;
		
		BlackboardComp->ClearValue(TargetItemKey.SelectedKeyName);
		SP->GetGroundedItems().Remove(PerceivedItem);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
