#include "BTTask_CheckHouse.h"
#include "AIController.h"

#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTTask_CheckHouse::UBTTask_CheckHouse()
{
	NodeName = "BTT Check House";
}

EBTNodeResult::Type UBTTask_CheckHouse::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
		                                 TEXT("Invalid AiController"));
		return EBTNodeResult::Failed;
	}


	APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
		                                 TEXT("Invalid OwnerPawn"));
		return EBTNodeResult::Failed;
	}

	UStudentPerceptorGeertsWarre* SP =
		Cast<UStudentPerceptorGeertsWarre>(OwnerPawn->GetComponentByClass(UStudentPerceptorGeertsWarre::StaticClass()));
	if (!SP)  return EBTNodeResult::Failed;
	
	SP->MarkCurrentHouseAsChecked();
	return EBTNodeResult::Succeeded;
}
