#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EvaluateZombieMemory.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_EvaluateZombieMemory : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EvaluateZombieMemory();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
