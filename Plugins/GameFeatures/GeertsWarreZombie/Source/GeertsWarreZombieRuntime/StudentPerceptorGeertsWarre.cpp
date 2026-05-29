// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptorGeertsWarre.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Common/InventoryComponent.h"




UStudentPerceptorGeertsWarre::UStudentPerceptorGeertsWarre()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptorGeertsWarre::BeginPlay()
{
	Super::BeginPlay();

	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptorGeertsWarre::OnPerceptionUpdated);
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn)
	{
		UInventoryComponent* TargetInventory = OwnerPawn->GetComponentByClass<UInventoryComponent>();
		PickupRange = TargetInventory->GetPickupRange();
	}
}

void UStudentPerceptorGeertsWarre::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	if (!AIController) return;

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp) return;

	FVector PlayerLocation = OwnerPawn->GetActorLocation();

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

	if (TrackedZombies.Num() > 0)
	{
		FVector CombinedAwayDir = FVector::ZeroVector;

		for (AActor* Zombie : TrackedZombies)
		{
			if (Zombie)
			{
				FVector ZombieLocation = Zombie->GetActorLocation();
				FVector AwayFromZombie = (PlayerLocation - ZombieLocation).GetSafeNormal();

				float Distance = FVector::Dist(PlayerLocation, ZombieLocation);
				float Weight = 1.0f / FMath::Max(Distance, 1.0f);

				CombinedAwayDir += AwayFromZombie * Weight;
			}
		}

		CombinedAwayDir = CombinedAwayDir.GetSafeNormal();
		FVector FleeLocation = PlayerLocation + (CombinedAwayDir * 1200.f);

		BlackboardComp->SetValueAsVector(TEXT("FleeLocation"), FleeLocation);

		//Debug
		DrawDebugLine(GetWorld(), PlayerLocation, FleeLocation, FColor::Emerald, false, -1.f, 0, 4.f);
	}
	else
	{
		FVector CurrentFlee = BlackboardComp->GetValueAsVector(TEXT("FleeLocation"));
		if (!CurrentFlee.IsZero())
		{
			BlackboardComp->ClearValue(TEXT("FleeLocation"));
		}
	}
}

void UStudentPerceptorGeertsWarre::UpdateBlackboardData(AActor* Actor, const FAIStimulus& Stimulus)
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

	if (bIsZombie)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			TrackedZombies.Add(Actor);
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, TEXT("Zombie spotted and added to memory!"));
		}
	}
	else
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (!OwnerPawn) return;

		AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
		if (!AIController) return;

		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (!BlackboardComp) return;

		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
			BlackboardComp->SetValueAsVector(TEXT("TargetLocation"), Actor->GetActorLocation());

			PickUp(Actor);			
		}
		else
		{
			BlackboardComp->ClearValue(TEXT("TargetActor"));
			BlackboardComp->ClearValue(TEXT("TargetLocation"));
		}
	}
}

void UStudentPerceptorGeertsWarre::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || Actor->GetName().Contains(TEXT("Garbage"))) return;
	UpdateBlackboardData(Actor, Stimulus);
}

void UStudentPerceptorGeertsWarre::PickUp(AActor* Actor)
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return;
    
    const float DistanceToTarget = FVector::Dist(OwnerPawn->GetActorLocation(), Actor->GetActorLocation());
    if (DistanceToTarget <= PickupRange)
    {
        UInventoryComponent* TargetInventory = OwnerPawn->GetComponentByClass<UInventoryComponent>();

        if (TargetInventory)
        {
            ABaseItem* PerceivedItem = Cast<ABaseItem>(Actor);

            if (PerceivedItem)
            {
                // Get a reference to the inventory array
                const TArray<ABaseItem*>& InventoryItems = TargetInventory->GetInventory();
                
                int32 TargetSlot = -1;

                // Loop through to find the first empty slot (nullptr)
                for (int32 i = 0; i < InventoryItems.Num(); i++)
                {
                    if (InventoryItems[i] == nullptr)
                    {
                        TargetSlot = i; // Found an empty slot!
                        break;          // Stop looking, we want the first available one
                    }
                }

                // If TargetSlot is still -1, it means all slots are full
                if (TargetSlot != -1)
                {
                    bool bGrabSuccessful = TargetInventory->GrabItem(TargetSlot, PerceivedItem);

                    if (bGrabSuccessful)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
                            FString::Printf(TEXT("Item grabbed successfully into Slot %d!"), TargetSlot));
                        
                        // Print out the updated inventory state
                        for (int32 i = 0; i < InventoryItems.Num(); i++)
                        {
                            ABaseItem* CurrentItem = InventoryItems[i]; 
                            FString SlotStatus = CurrentItem ? CurrentItem->GetName() : TEXT("Empty");
                            FString DebugMessage = FString::Printf(TEXT("Slot %d: %s"), i, *SlotStatus);
                
                            GEngine->AddOnScreenDebugMessage(
                                -1, 
                                10.f, 
                                CurrentItem ? FColor::Cyan : FColor::Orange, 
                                DebugMessage
                            );
                        }
                    }
                    else
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
                            TEXT("Grab failed! Item initialization or constraint issue."));
                    }
                }
                else
                {
                    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
                        TEXT("Grab failed! Inventory is completely full."));
                }
            }
        }
    }
}