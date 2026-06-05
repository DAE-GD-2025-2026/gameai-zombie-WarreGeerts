#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTTask_FindFleeLocation.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_FindFleeLocation : public UBTService
{
	GENERATED_BODY()

public:
	UBTTask_FindFleeLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector FleeLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Fleeing")
	float MemoryRadius = 2000.f;
};