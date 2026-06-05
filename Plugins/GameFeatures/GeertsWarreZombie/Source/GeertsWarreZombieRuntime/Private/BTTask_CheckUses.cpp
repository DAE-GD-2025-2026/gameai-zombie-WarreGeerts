#include "BTTask_CheckUses.h"

#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTTask_CheckUses::UBTTask_CheckUses()
{
	NodeName = "BTT Check Uses";
}

EBTNodeResult::Type UBTTask_CheckUses::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UInventoryComponent* TargetInventory = OwnerPawn->GetComponentByClass<UInventoryComponent>();
	if (!TargetInventory) return EBTNodeResult::Failed;

	const TArray<ABaseItem*>& InventoryItems = TargetInventory->GetInventory();

	for (int Index = InventoryItems.Num() -1; Index >= 0; --Index)
	{
		const ABaseItem* Item = InventoryItems[Index];
		if (!Item) continue;

		if (Item->GetValue() <= 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, 
			 FString::Printf(TEXT("Removing empty item: %s at index %d"), *Item->GetName(), Index));
			TargetInventory->RemoveItem(Index);
		}
	}

	return EBTNodeResult::Succeeded;
}
