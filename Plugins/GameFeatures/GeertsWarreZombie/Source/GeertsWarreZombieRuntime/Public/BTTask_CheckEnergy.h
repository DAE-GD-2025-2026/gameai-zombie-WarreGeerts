#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckEnergy.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_CheckEnergy : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CheckEnergy();
	
protected:
	virtual auto ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) -> EBTNodeResult::Type override;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PlayerEnergyKey;
};