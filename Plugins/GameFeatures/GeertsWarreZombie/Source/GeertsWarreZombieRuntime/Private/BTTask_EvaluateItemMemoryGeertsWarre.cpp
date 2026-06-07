#include "BTTask_EvaluateItemMemoryGeertsWarre.h"
#include "AIController.h"
#include "IDetailTreeNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

class UInventoryComponent;


UBTTask_EvaluateItemMemoryGeertsWarre::UBTTask_EvaluateItemMemoryGeertsWarre()
{
	NodeName = "BTT Evaluate Item Memory";
}

EBTNodeResult::Type UBTTask_EvaluateItemMemoryGeertsWarre::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	auto GroundedItems = SP->GetGroundedItems();
	auto InventoryItems = TargetInventory->GetInventory();

	bool fullPistols = false;
	bool fullShotguns = false;
	bool fullFood = false;
	bool fullMedkits = false;

	for (AActor* Item : InventoryItems)
	{
		if (Item == nullptr)
		{
			continue;
		}
		
		FString ItemName = Item->GetClass()->GetName();

		if (ItemName.Contains(TEXT("Pistol")))
		{
			int ShotgunCount = BlackboardComp->GetValueAsInt(PistolAmountKey.SelectedKeyName);
			int MaxShotgunCount = BlackboardComp->GetValueAsInt(MaxPistolAmountKey.SelectedKeyName);
			if (ShotgunCount >= MaxShotgunCount)
			{
				fullPistols = true;
			}
		}
		if (ItemName.Contains(TEXT("Shotgun")))
		{
			int ShotgunCount = BlackboardComp->GetValueAsInt(ShotGunAmountKey.SelectedKeyName);
			int MaxShotgunCount = BlackboardComp->GetValueAsInt(MaxShotGunAmountKey.SelectedKeyName);
			if (ShotgunCount >= MaxShotgunCount)
			{
				fullShotguns = true;
			}
		}
		if (ItemName.Contains(TEXT("Food")))
		{
			int FoodCount = BlackboardComp->GetValueAsInt(FoodAmountKey.SelectedKeyName);
			int MaxFoodCount = BlackboardComp->GetValueAsInt(MaxFoodAmountKey.SelectedKeyName);
			if (FoodCount >= MaxFoodCount)
			{
				fullFood = true;
			}
		}
		if (ItemName.Contains(TEXT("Medkit")))
		{
			int MedkitCount = BlackboardComp->GetValueAsInt(MedkitAmountKey.SelectedKeyName);
			int MaxMedkitCount = BlackboardComp->GetValueAsInt(MaxMedkitAmountKey.SelectedKeyName);
			if (MedkitCount >= MaxMedkitCount)
			{
				fullMedkits = true;
			}
		}
	}

	for (AActor* Item : GroundedItems)
	{
		if (Item == nullptr)
		{
			continue;
		}

		if (SetTarget(BlackboardComp, Item, "Shotgun", fullShotguns)) break;
		if (SetTarget(BlackboardComp, Item, "Pistol", fullPistols)) break;
		if (SetTarget(BlackboardComp, Item, "Food", fullFood)) break;
		if (SetTarget(BlackboardComp, Item, "Medkit", fullMedkits)) break;
	}
	return EBTNodeResult::Succeeded;
}


bool UBTTask_EvaluateItemMemoryGeertsWarre::SetTarget(UBlackboardComponent* BC, AActor* Item, const FString& ContainName,
                                           const bool IsFull)
{
	if (IsFull) return false;
	if (!Item) return false;

	const FString ItemName = Item->GetClass()->GetName();

	if (ItemName.Contains(ContainName))
	{
		BC->SetValueAsObject(TEXT("TargetItem"), Item);
		BC->SetValueAsVector(TEXT("ItemLocation"), Item->GetActorLocation());
		return true;
	}
	return false;
}
