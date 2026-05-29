// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "StudentPerceptorGeertsWarre.generated.h"
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GEERTSWARREZOMBIERUNTIME_API UStudentPerceptorGeertsWarre : public UActorComponent
{
	GENERATED_BODY()

	// Tracked zombies that the player actively "remembers"
	UPROPERTY()
	TSet<AActor*> TrackedZombies;

	// The maximum radius the player will remember a zombie after losing sight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Fleeing", meta = (AllowPrivateAccess = "true"))
	float MemoryRadius = 2000.f;
	
	virtual void PickUp(AActor* Actor);
	float PickupRange;
	
	
public:
	// Sets default values for this component's properties
	UStudentPerceptorGeertsWarre();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void UpdateBlackboardData(AActor* Actor, const FAIStimulus& Stimulus);
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
