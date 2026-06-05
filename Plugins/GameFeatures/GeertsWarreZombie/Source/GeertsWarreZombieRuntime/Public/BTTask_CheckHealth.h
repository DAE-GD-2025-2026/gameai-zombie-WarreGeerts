#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckHealth.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_CheckHealth : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CheckHealth();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PlayerHealthKey;
};