#include "BTTask_PickUpItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"
#include "Engine/Engine.h"


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

	
	AActor* ItemActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetItemKey.SelectedKeyName));
	if (!ItemActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
											 TEXT("Invalid ItemActor"));
		BlackboardComp->ClearValue(TargetItemKey.SelectedKeyName);
		return EBTNodeResult::Failed;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green,
	                                 FString::Printf(
		                                 TEXT("Attempting to pick up item: %s"),
		                                 *ItemActor->GetName()));

	UInventoryComponent* TargetInventory = OwnerPawn->GetComponentByClass<UInventoryComponent>();
	if (!TargetInventory) return EBTNodeResult::Failed;

	PickupRange = TargetInventory->GetPickupRange();

	const float DistanceToTarget = FVector::Dist(OwnerPawn->GetActorLocation(), ItemActor->GetActorLocation());
	if (DistanceToTarget > PickupRange)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
		                                 TEXT("Pickup Failed: Item out of range!"));
		return EBTNodeResult::Failed;
	}

	ABaseItem* PerceivedItem = Cast<ABaseItem>(ItemActor);
	if (!PerceivedItem) return EBTNodeResult::Failed;

	const TArray<ABaseItem*>& InventoryItems = TargetInventory->GetInventory();
	int32 TargetSlot = -1;

	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
		if (InventoryItems[i] == nullptr)
		{
			TargetSlot = i;
			break;
		}
	}

	if (TargetSlot != -1)
	{
		bool bGrabSuccessful = TargetInventory->GrabItem(TargetSlot, PerceivedItem);
		if (!bGrabSuccessful) return EBTNodeResult::Failed;

		GEngine->AddOnScreenDebugMessage(
			-1, 2.f, FColor::Cyan,
			FString::Printf(TEXT("Item grabbed successfully into Slot %d!"), TargetSlot));

		for (int32 i = 0; i < InventoryItems.Num(); i++)
		{
			const ABaseItem* CurrentItem = InventoryItems[i];
			FString SlotStatus = CurrentItem ? CurrentItem->GetName() : TEXT("Empty");
			FString DebugMessage = FString::Printf(TEXT("Slot %d: %s"), i, *SlotStatus);

			GEngine->AddOnScreenDebugMessage(-1, 10.f,
			                                 CurrentItem ? FColor::Cyan : FColor::Orange, DebugMessage);
		}

		BlackboardComp->ClearValue(TargetItemKey.SelectedKeyName);
		return EBTNodeResult::Succeeded;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
	                                 TEXT("Pickup Failed!"));
	return EBTNodeResult::Failed;
}
