#include "BTTask_CheckInventory.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"
#include "Common/InventoryComponent.h"

UBTTask_CheckInventory::UBTTask_CheckInventory()
{
	NodeName = "BTT Check Inventory";
}

EBTNodeResult::Type UBTTask_CheckInventory::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	APawn* OwnerPawn = AIController->GetPawn();
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
	bool bHasWeapon = false;
	int AmountShotguns = 0;
	int AmountPistols = 0;
	int AmountFood = 0;
	int AmountMedkits = 0;

	for (const ABaseItem* Item : InventoryItems)
	{
		if (!Item) continue;

		FString ItemClassName = Item->GetClass()->GetName();

		if (ItemClassName.Contains(TEXT("Shotgun")) || ItemClassName.Contains(TEXT("Pistol")))
		{
			bHasWeapon = true;

			if (ItemClassName.Contains(TEXT("Shotgun")))
			{
				AmountShotguns++;
			}
			else
			{
				AmountPistols++;
			}
		}

		if (ItemClassName.Contains(TEXT("Food")))
		{
			AmountFood++;
		}

		if (ItemClassName.Contains(TEXT("Medkit")))
		{
			AmountMedkits++;
		}
	}

	BlackboardComp->SetValueAsBool(HasWeaponKey.SelectedKeyName, bHasWeapon);
	BlackboardComp->SetValueAsInt(ShotGunAmountKey.SelectedKeyName, AmountShotguns);
	BlackboardComp->SetValueAsInt(PistolAmountKey.SelectedKeyName, AmountPistols);
	BlackboardComp->SetValueAsInt(FoodAmountKey.SelectedKeyName, AmountFood);
	BlackboardComp->SetValueAsInt(MedkitAmountKey.SelectedKeyName, AmountMedkits);


	return EBTNodeResult::Succeeded;
}
