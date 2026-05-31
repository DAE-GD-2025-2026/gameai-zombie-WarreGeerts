#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EvaluateItemMemory.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_EvaluateItemMemory : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EvaluateItemMemory();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};