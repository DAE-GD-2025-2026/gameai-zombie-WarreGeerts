#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Rotate.generated.h"

UCLASS()
class GEERTSWARREZOMBIERUNTIME_API UBTTask_Rotate : public UBTTaskNode
{
	GENERATED_BODY()
    
	FRotator StartRotation;
	FRotator TargetRotation;
	float DegreesTurnedSoFar;
	TWeakObjectPtr<APawn> CachedPawn;
	TWeakObjectPtr<APlayerController> CachedPlayerController;

public:
	UBTTask_Rotate();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void RestorePlayerInput() const;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float AngleToRotate = 360.f;
    
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationSpeed = 180.f;
};