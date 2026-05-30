#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindFleeLocation.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_FindFleeLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindFleeLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector FleeLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Fleeing")
	float MemoryRadius = 2000.f;
};