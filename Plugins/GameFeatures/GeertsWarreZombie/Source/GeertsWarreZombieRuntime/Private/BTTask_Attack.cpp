#include "BTTask_Attack.h"

#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "BTT Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	for (int32 Index = 0; Index < InventoryItems.Num(); ++Index)
	{
		const ABaseItem* Item = InventoryItems[Index];
		if (!Item) continue;

		FString ItemClassName = Item->GetClass()->GetName();

		if (ItemClassName.Contains(TEXT("Shotgun")) || ItemClassName.Contains(TEXT("Pistol")))
		{
			TargetInventory->UseItem(Index);
			
			SP->GetTrackedZombies().Remove(nullptr);
       
			for (auto It = SP->GetTrackedZombies().CreateIterator(); It; ++It)
			{
				AActor* ZombieActor = *It;
				if (!IsValid(ZombieActor))
				{
					It.RemoveCurrent();
				}
			}
			
			
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
