#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckItemMemory.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_CheckItemMemory : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CheckItemMemory();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};