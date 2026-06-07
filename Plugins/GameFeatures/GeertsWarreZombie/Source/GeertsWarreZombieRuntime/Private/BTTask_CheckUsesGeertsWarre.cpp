#include "BTTask_CheckUsesGeertsWarre.h"

#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTTask_CheckUsesGeertsWarre::UBTTask_CheckUsesGeertsWarre()
{
	NodeName = "BTT Check Uses";
}

EBTNodeResult::Type UBTTask_CheckUsesGeertsWarre::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	const APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn)
	{
		return EBTNodeResult::Failed;
	}

	UStudentPerceptorGeertsWarre* SP =
		Cast<UStudentPerceptorGeertsWarre>(OwnerPawn->GetComponentByClass(UStudentPerceptorGeertsWarre::StaticClass()));
	if (!SP)
	{
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
			TargetInventory->RemoveItem(Index);
		}
	}

	return EBTNodeResult::Succeeded;
}
