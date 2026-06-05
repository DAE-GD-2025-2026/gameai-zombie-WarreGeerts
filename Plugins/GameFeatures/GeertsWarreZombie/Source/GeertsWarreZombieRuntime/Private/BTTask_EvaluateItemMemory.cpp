#include "BTTask_EvaluateItemMemory.h"
#include "AIController.h"
#include "IDetailTreeNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

class UInventoryComponent;


UBTTask_EvaluateItemMemory::UBTTask_EvaluateItemMemory()
{
	NodeName = "BTT Evaluate Item Memory";
}

EBTNodeResult::Type UBTTask_EvaluateItemMemory::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	auto GroundedItems = SP->GetGroundedItems();
	auto InventoryItems = TargetInventory->GetInventory();

	bool fullPistols = false;
	bool fullShotguns = false;

	for (AActor* Item : InventoryItems)
	{
		if (Item == nullptr)
		{
			continue;
		}
		
		FString ItemName = Item->GetName();


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
	}

	for (AActor* Item : GroundedItems)
	{
		if (Item == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			                                 FString::Printf(TEXT("Invalid item")));
			continue;
		}

		if (SetTarget(BlackboardComp, Item, "Shotgun", fullShotguns)) break;
		if (SetTarget(BlackboardComp, Item, "Pistol", fullPistols)) break;
	}
	return EBTNodeResult::Succeeded;
}


bool UBTTask_EvaluateItemMemory::SetTarget(UBlackboardComponent* BC, AActor* Item, const FString& ContainName,
                                           const bool IsFull)
{
	if (IsFull) return false;
	if (!Item) return false;

	const FString ItemName = Item->GetName();

	if (ItemName.Contains(ContainName))
	{
		BC->SetValueAsObject(TEXT("TargetItem"), Item);
		BC->SetValueAsVector(TEXT("ItemLocation"), Item->GetActorLocation());
		return true;
	}
	return false;
}
