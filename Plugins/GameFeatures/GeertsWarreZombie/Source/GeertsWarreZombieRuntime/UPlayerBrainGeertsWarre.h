#pragma once
#include "UPlayerBrainGeertsWarre.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GEERTSWARREZOMBIERUNTIME_API UPlayerBrainGeertsWarre : public UActorComponent
{
public:
	UPlayerBrainGeertsWarre();

	virtual bool ZombieCheck(AActor* Actor);
	virtual void ForgetZombies(FVector PlayerLocation);

	virtual TSet<AActor*>& GetTrackedZombies() { return TrackedZombies; }

private:
	GENERATED_BODY()

	UPROPERTY()
	TSet<AActor*> TrackedZombies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Fleeing", meta = (AllowPrivateAccess = "true"))
	float MemoryRadius = 2000.f;
};
