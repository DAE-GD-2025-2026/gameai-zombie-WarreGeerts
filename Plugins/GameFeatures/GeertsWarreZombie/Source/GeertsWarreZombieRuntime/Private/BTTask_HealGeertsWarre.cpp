#include "BTTask_HealGeertsWarre.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTTask_HealGeertsWarre::UBTTask_HealGeertsWarre()
{
	NodeName = "BTT Heal Player";
}

EBTNodeResult::Type UBTTask_HealGeertsWarre::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

		if (ItemClassName.Contains(TEXT("Medkit")))
		{
			auto currentHealth = BlackboardComp->GetValueAsInt(PlayerHealthKey.SelectedKeyName);
			auto healValue = Item->GetValue();

			UHealthComponent* TargetHealth = OwnerPawn->GetComponentByClass<UHealthComponent>();
			if (!TargetHealth) return EBTNodeResult::Failed;

			if (currentHealth <= TargetHealth->GetMaxHealth() - healValue)
			{
				TargetInventory->UseItem(Index);
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
