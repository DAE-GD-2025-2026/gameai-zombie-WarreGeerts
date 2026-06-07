#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckEnergyGeertsWarre.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_CheckEnergyGeertsWarre : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CheckEnergyGeertsWarre();
	
protected:
	virtual auto ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) -> EBTNodeResult::Type override;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PlayerEnergyKey;
};