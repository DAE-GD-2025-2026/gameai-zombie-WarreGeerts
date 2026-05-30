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

	bool ZombieCheck(AActor* Actor);
	bool HouseCheck(AActor* Actor);
	
	TSet<AActor*>& GetTrackedZombies() { return TrackedZombies; }
	TSet<AActor*>& GetEnteredHouses() { return EnteredHouses; }
private:
	GENERATED_BODY()

	UPROPERTY()
	TSet<AActor*> TrackedZombies;
	
	UPROPERTY()
	TSet<AActor*> EnteredHouses;
};
