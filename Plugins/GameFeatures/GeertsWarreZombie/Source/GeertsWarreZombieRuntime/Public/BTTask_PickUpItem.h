#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PickUpItem.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_PickUpItem : public UBTTaskNode
{
	GENERATED_BODY()
    
public:
	UBTTask_PickUpItem();
    
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetItemKey;

private:
	float PickupRange = 200.f;
};