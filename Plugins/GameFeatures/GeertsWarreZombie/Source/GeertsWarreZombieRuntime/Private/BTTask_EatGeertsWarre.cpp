#include "BTTask_EatGeertsWarre.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTTask_EatGeertsWarre::UBTTask_EatGeertsWarre()
{
	NodeName = "BTT Eat";
}

EBTNodeResult::Type UBTTask_EatGeertsWarre::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	const TArray<ABaseItem*>& InventoryItems = TargetInventory->GetInventory();

	for (int Index = 0; Index < InventoryItems.Num(); ++Index)
	{
		const ABaseItem* Item = InventoryItems[Index];
		if (!Item) continue;

		FString ItemClassName = Item->GetClass()->GetName();

		if (ItemClassName.Contains(TEXT("Food")))
		{
			auto currentEnergy = BlackboardComp->GetValueAsFloat(PlayerEnergyKey.SelectedKeyName);
			auto foodValue = Item->GetValue();

			UStaminaComponent* TargetEnergy = OwnerPawn->GetComponentByClass<UStaminaComponent>();
			if (!TargetEnergy) return EBTNodeResult::Failed;

			if (currentEnergy <= TargetEnergy->GetMaxStamina() - foodValue)
			{
				TargetInventory->UseItem(Index);
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
