// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPlayerBrainGeertsWarre.h"
#include "UPlayerInventoryGeertsWarre.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "StudentPerceptorGeertsWarre.generated.h"
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GEERTSWARREZOMBIERUNTIME_API UStudentPerceptorGeertsWarre : public UActorComponent
{
public:
	UStudentPerceptorGeertsWarre();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void UpdateBlackboardData(AActor* Actor, const FAIStimulus& Stimulus);
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
private:
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerBrainGeertsWarre> Brain;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerInventoryGeertsWarre> Inventory;
};
