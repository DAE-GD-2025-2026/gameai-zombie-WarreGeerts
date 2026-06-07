#include "BTTask_EvaluateZombieMemoryGeertsWarre.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"


UBTTask_EvaluateZombieMemoryGeertsWarre::UBTTask_EvaluateZombieMemoryGeertsWarre()
{
	NodeName = "BTT Evaluate Zombie Memory";
}

EBTNodeResult::Type UBTTask_EvaluateZombieMemoryGeertsWarre::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	const APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn)
	{
		return EBTNodeResult::Failed;
	}

	UStudentPerceptorGeertsWarre* SP =
		Cast<UStudentPerceptorGeertsWarre>(OwnerPawn->GetComponentByClass(UStudentPerceptorGeertsWarre::StaticClass()));
	if (!SP)
	{
		return EBTNodeResult::Failed;
	}

	SP->GetTrackedZombies().Remove(nullptr);
	for (auto It = SP->GetTrackedZombies().CreateIterator(); It; ++It)
	{
		if (!IsValid(*It))
		{
			It.RemoveCurrent();
			BlackboardComp->ClearValue(TEXT("TargetZombie"));
			BlackboardComp->ClearValue(TEXT("ChargeLocation"));
			BlackboardComp->SetValueAsBool(TEXT("ZombieInRange"), false);
		}
	}

	BlackboardComp->SetValueAsBool(TEXT("HasTrackedZombies"), SP->GetTrackedZombies().Num() > 0);

	return EBTNodeResult::Succeeded;
}
