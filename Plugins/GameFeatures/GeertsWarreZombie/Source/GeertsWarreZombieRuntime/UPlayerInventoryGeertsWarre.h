#pragma once
#include "UPlayerInventoryGeertsWarre.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GEERTSWARREZOMBIERUNTIME_API UPlayerInventoryGeertsWarre : public UActorComponent
{
public:
	UPlayerInventoryGeertsWarre();

	virtual bool PickUp(AActor* Actor);	
private:
	GENERATED_BODY()
	float PickupRange;

};
