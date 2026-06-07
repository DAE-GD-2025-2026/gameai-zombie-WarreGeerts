#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTTask_ChargeGeertsWarre.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_ChargeGeertsWarre : public UBTService
{
	GENERATED_BODY()

public:
	UBTTask_ChargeGeertsWarre();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float Distance = 150.f;
	
	UPROPERTY(EditAnywhere, Category = "Default")
	float ChaseRange = 500.f;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ChargeLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetZombieKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector InRangeKey;
	

};
