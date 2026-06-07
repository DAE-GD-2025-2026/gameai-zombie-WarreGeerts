#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_AvoidZonesGeertsWarre.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTService_AvoidZones : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_AvoidZones();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Default")
	float DangerRadius = 300.f; 

	UPROPERTY(EditAnywhere, Category = "Default")
	float FleeDistance = 500.f; 

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector FleeLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector InZoneDangerKey; 
};
