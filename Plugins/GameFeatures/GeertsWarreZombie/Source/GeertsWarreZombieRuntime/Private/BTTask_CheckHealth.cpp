#include "BTTask_CheckHealth.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/HealthComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTTask_CheckHealth::UBTTask_CheckHealth()
{
	NodeName = "BTT Check Health";
}

EBTNodeResult::Type UBTTask_CheckHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	APawn* OwnerPawn = AIController->GetPawn();
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
	
	UHealthComponent* TargetHealth = OwnerPawn->GetComponentByClass<UHealthComponent>();
	if (!TargetHealth) return EBTNodeResult::Failed;
	
	BlackboardComp->SetValueAsInt(PlayerHealthKey.SelectedKeyName,TargetHealth->GetHealth());
	
	return EBTNodeResult::Succeeded;
}
