#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Charge.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_Charge : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Charge();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float Distance = 150.f;
};