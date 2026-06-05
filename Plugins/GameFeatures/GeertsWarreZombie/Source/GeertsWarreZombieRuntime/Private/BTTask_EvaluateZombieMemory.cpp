#include "BTTask_EvaluateZombieMemory.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"


UBTTask_EvaluateZombieMemory::UBTTask_EvaluateZombieMemory()
{
	NodeName = "BTT Evaluate Zombie Memory";
}

EBTNodeResult::Type UBTTask_EvaluateZombieMemory::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	SP->GetTrackedZombies().Remove(nullptr);
	for (auto It = SP->GetTrackedZombies().CreateIterator(); It; ++It)
	{
		if (!IsValid(*It)) It.RemoveCurrent();
	}

	BlackboardComp->SetValueAsBool(TEXT("HasTrackedZombies"), SP->GetTrackedZombies().Num() > 0);
	
	return EBTNodeResult::Succeeded;
}
