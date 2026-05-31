#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckUses.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_CheckUses : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CheckUses();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};