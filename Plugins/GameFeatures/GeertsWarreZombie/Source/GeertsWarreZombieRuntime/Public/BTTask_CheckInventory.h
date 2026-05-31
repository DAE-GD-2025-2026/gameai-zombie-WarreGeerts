#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckInventory.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_CheckInventory : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CheckInventory();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int MaxAmountShotGun = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int MaxAmountPistol = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int MaxAmountFood = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int MaxAmountMedkit = 1;
};
