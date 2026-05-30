#include "UPlayerInventoryGeertsWarre.h"

#include "Common/InventoryComponent.h"

UPlayerInventoryGeertsWarre::UPlayerInventoryGeertsWarre()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UPlayerInventoryGeertsWarre::PickUp(AActor* Actor)
{
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return false;

	UInventoryComponent* TargetInventory = OwnerPawn->GetComponentByClass<UInventoryComponent>();
	if (!TargetInventory) return false;
	PickupRange = TargetInventory->GetPickupRange();
	
	const float DistanceToTarget = FVector::Dist(OwnerPawn->GetActorLocation(), Actor->GetActorLocation());
	if (DistanceToTarget > PickupRange) return false;

	ABaseItem* PerceivedItem = Cast<ABaseItem>(Actor);
	if (!PerceivedItem) return false;
	
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
		if (!bGrabSuccessful) return false;

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
										 FString::Printf(
											 TEXT("Item grabbed successfully into Slot %d!"),
											 TargetSlot));

		for (int32 i = 0; i < InventoryItems.Num(); i++)
		{
			const ABaseItem* CurrentItem = InventoryItems[i];
			FString SlotStatus = CurrentItem ? CurrentItem->GetName() : TEXT("Empty");
			FString DebugMessage = FString::Printf(TEXT("Slot %d: %s"), i, *SlotStatus);

			GEngine->AddOnScreenDebugMessage(
				-1,
				10.f,
				CurrentItem ? FColor::Cyan : FColor::Orange,
				DebugMessage
			);
		}
		return true;
	}
	return false;
}
