#include "BTTask_EvaluateItemMemory.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTTask_EvaluateItemMemory::UBTTask_EvaluateItemMemory()
{
	NodeName = "BTT Evaluate Item Memory";
}

EBTNodeResult::Type UBTTask_EvaluateItemMemory::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
		                                 TEXT("Invalid AiController or BlackboardComp"));
		return EBTNodeResult::Failed;
	}

	const APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
		                                 TEXT("Invalid OwnerPawn"));
		return EBTNodeResult::Failed;
	}

	UStudentPerceptorGeertsWarre* SP =
		Cast<UStudentPerceptorGeertsWarre>(OwnerPawn->GetComponentByClass(UStudentPerceptorGeertsWarre::StaticClass()));
	if (!SP)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
		                                 TEXT("Invalid UStudentPerceptorGeertsWarre"));
		return EBTNodeResult::Failed;
	}

	auto GroundedItems = SP->GetGroundedItems();

	for (AActor* Item : GroundedItems)
	{		
		if (Item == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
											 FString::Printf(TEXT("Invalid item")));
			continue;
		};

		FString ItemName = Item->GetName();

		if (ItemName.Contains(TEXT("Shotgun")) || ItemName.Contains(TEXT("Pistol")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			                                 FString::Printf(TEXT("Found weapon in memory: %s"), *ItemName));


			BlackboardComp->SetValueAsObject(TEXT("TargetItem"), Item);
			BlackboardComp->SetValueAsVector(TEXT("ItemLocation"), Item->GetActorLocation());
			break;
		}
	}
	return EBTNodeResult::Succeeded;
}
