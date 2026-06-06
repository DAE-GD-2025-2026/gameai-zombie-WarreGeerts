#include "BTTask_CheckEnergy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/StaminaComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTTask_CheckEnergy::UBTTask_CheckEnergy()
{
	NodeName = "BTT Check Energy";
}

EBTNodeResult::Type UBTTask_CheckEnergy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	UStaminaComponent* TargetEnergy = OwnerPawn->GetComponentByClass<UStaminaComponent>();
	if (!TargetEnergy) return EBTNodeResult::Failed;
	
	BlackboardComp->SetValueAsFloat(PlayerEnergyKey.SelectedKeyName,TargetEnergy->GetCurrentStamina());
	
	return EBTNodeResult::Succeeded;
}
