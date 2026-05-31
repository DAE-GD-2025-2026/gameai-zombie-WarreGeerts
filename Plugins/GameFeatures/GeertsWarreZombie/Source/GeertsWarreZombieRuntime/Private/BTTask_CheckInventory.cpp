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
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
										 TEXT("Invalid AiController or BlackboardComp"));
		return EBTNodeResult::Failed;
	}

	APawn* OwnerPawn = AIController->GetPawn();
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
	bool bHasWeapon = false;
	int AmountShotguns = 0;
	int AmountPistols = 0;

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
			}else
			{
				AmountPistols++;
			}			
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
				FString::Printf(TEXT("Weapon Found: %s"), *ItemClassName));
		}
		
		//check for other items as well. . .
	}

	//set booleans for other items as well, so they can be used if needed. . .
	if (bHasWeapon)
	{
		BlackboardComp->SetValueAsBool(TEXT("HasWeapon"), bHasWeapon);
		BlackboardComp->SetValueAsInt(TEXT("AmountShotguns"), AmountShotguns);
		BlackboardComp->SetValueAsInt(TEXT("AmountPistols"), AmountPistols);
	}
	
	
	
	return EBTNodeResult::Succeeded;
}
