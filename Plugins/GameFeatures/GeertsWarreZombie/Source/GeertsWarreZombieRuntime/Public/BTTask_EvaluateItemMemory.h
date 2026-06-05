#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EvaluateItemMemory.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_EvaluateItemMemory : public UBTTaskNode
{
	GENERATED_BODY()
	bool SetTarget(UBlackboardComponent* BC, AActor* Item, const FString& ContainName, const bool IsFull);

public:
	UBTTask_EvaluateItemMemory();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//weapons
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ShotGunAmountKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxShotGunAmountKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PistolAmountKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxPistolAmountKey;
	
	//utilities
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector FoodAmountKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxFoodAmountKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MedkitAmountKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxMedkitAmountKey;
};
