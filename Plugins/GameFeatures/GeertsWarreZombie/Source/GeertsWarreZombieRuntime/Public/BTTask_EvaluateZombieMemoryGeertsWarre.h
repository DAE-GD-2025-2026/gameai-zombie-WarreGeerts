#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EvaluateZombieMemoryGeertsWarre.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_EvaluateZombieMemoryGeertsWarre : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EvaluateZombieMemoryGeertsWarre();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
