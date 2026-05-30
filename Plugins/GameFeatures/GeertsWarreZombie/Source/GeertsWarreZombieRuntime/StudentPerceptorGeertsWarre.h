// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "StudentPerceptorGeertsWarre.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GEERTSWARREZOMBIERUNTIME_API UStudentPerceptorGeertsWarre : public UActorComponent
{
public:
	UStudentPerceptorGeertsWarre();
	virtual void BeginPlay() override;
    
	void UpdateBlackboardData(AActor* Actor, const FAIStimulus& Stimulus);
    
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	static bool ZombieCheck(AActor* Actor);

	// This allows your BTTask_FindFleeLocation to access the list!
	TSet<AActor*>& GetTrackedZombies() { return TrackedZombies; }

private:
	GENERATED_BODY()

	UPROPERTY()
	TSet<AActor*> TrackedZombies;
};
