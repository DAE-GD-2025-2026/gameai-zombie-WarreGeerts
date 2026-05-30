#include "UPlayerBrainGeertsWarre.h"

UPlayerBrainGeertsWarre::UPlayerBrainGeertsWarre()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UPlayerBrainGeertsWarre::ZombieCheck(AActor* Actor)
{
	bool bIsZombie = false;
	if (const APawn* Pawn = Cast<APawn>(Actor))
	{
		if (const AController* Controller = Pawn->GetController())
		{
			if (Controller->GetClass()->GetName().Contains(TEXT("BP_ZombieController")))
			{
				bIsZombie = true;
			}
		}
	}
	return bIsZombie;
}

void UPlayerBrainGeertsWarre::ForgetZombies(const FVector PlayerLocation)
{
	TArray<AActor*> ZombiesToRemove;
	for (AActor* Zombie : TrackedZombies)
	{
		if (!Zombie || !Zombie->IsValidLowLevel())
		{
			ZombiesToRemove.Add(Zombie);
			continue;
		}

		float Distance = FVector::Dist(PlayerLocation, Zombie->GetActorLocation());
		if (Distance > MemoryRadius)
		{
			ZombiesToRemove.Add(Zombie);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Forgot zombie: Out of radius."));
		}
	}

	for (AActor* ZombieToForget : ZombiesToRemove)
	{
		TrackedZombies.Remove(ZombieToForget);
	}
}
