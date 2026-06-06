#include "BTTask_Rotate.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Rotate::UBTTask_Rotate()
{
	NodeName = "BTT Rotate";
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Rotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		RestorePlayerInput();
		return EBTNodeResult::Failed;
	}


	APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn)
	{
		RestorePlayerInput();
		return EBTNodeResult::Failed;
	}

	if (APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
	{
		CachedPlayerController = PC;
		PC->SetIgnoreLookInput(true); 
		PC->SetIgnoreMoveInput(true); 
	}

	AIController->StopMovement();

	DegreesTurnedSoFar = 0.f;
	CachedPawn = OwnerPawn;

	StartRotation = OwnerPawn->GetActorRotation();
	TargetRotation = StartRotation;
	TargetRotation.Yaw = FRotator::NormalizeAxis(StartRotation.Yaw + AngleToRotate);
	
	if (ACharacter* OwnerChar = Cast<ACharacter>(OwnerPawn))
	{
		if (UCharacterMovementComponent* MoveComp = OwnerChar->GetCharacterMovement())
		{
			MoveComp->bOrientRotationToMovement = false;
			MoveComp->bUseControllerDesiredRotation = false;
		}
	}

	OwnerPawn->bUseControllerRotationYaw = false;

	return EBTNodeResult::InProgress;
}

void UBTTask_Rotate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!CachedPawn.IsValid())
	{
		RestorePlayerInput();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	APawn* OwnerPawn = CachedPawn.Get();
	const FRotator CurrentRot = OwnerPawn->GetActorRotation();

	const float StepThisFrame = FMath::Sign(AngleToRotate) * RotationSpeed * DeltaSeconds;

	const float Remaining = FMath::Abs(AngleToRotate) - DegreesTurnedSoFar;
	const float ActualStep = FMath::Min(FMath::Abs(StepThisFrame), Remaining) * FMath::Sign(StepThisFrame);
    
	DegreesTurnedSoFar += FMath::Abs(ActualStep);
	
	FRotator NewRot = CurrentRot;
	NewRot.Yaw = CurrentRot.Yaw + ActualStep;
	OwnerPawn->SetActorRotation(NewRot);

	if (CachedPlayerController.IsValid())
	{
		CachedPlayerController->SetControlRotation(NewRot);
	}

	if (DegreesTurnedSoFar >= FMath::Abs(AngleToRotate))
	{
		OwnerPawn->SetActorRotation(TargetRotation);
		RestorePlayerInput();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_Rotate::RestorePlayerInput() const
{
	if (CachedPlayerController.IsValid())
	{
		CachedPlayerController->ResetIgnoreLookInput();
		CachedPlayerController->ResetIgnoreMoveInput();
	}

	if (CachedPawn.IsValid())
	{
		APawn* Pawn = CachedPawn.Get();

		Pawn->bUseControllerRotationYaw = true;

		if (ACharacter* Char = Cast<ACharacter>(Pawn))
		{
			if (UCharacterMovementComponent* MoveComp = Char->GetCharacterMovement())
			{
				MoveComp->bOrientRotationToMovement = false;
				MoveComp->bUseControllerDesiredRotation = true;
			}
		}
	}
}
