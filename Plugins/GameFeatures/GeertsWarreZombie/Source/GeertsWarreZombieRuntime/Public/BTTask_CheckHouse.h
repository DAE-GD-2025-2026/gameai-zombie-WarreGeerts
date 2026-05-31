#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckHouse.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_CheckHouse : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CheckHouse();
	
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
