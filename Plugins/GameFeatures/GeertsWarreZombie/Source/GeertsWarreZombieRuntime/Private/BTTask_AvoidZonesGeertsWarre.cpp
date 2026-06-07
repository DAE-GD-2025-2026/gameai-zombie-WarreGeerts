#include "BTTask_AvoidZonesGeertsWarre.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GeertsWarreZombieRuntime/StudentPerceptorGeertsWarre.h"

UBTService_AvoidZones::UBTService_AvoidZones()
{
    NodeName = "BTS Avoid Zones";
    Interval = 0.05f;
    RandomDeviation = 0.01f;
}

void UBTService_AvoidZones::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!AIController || !BlackboardComp) return;

    const APawn* OwnerPawn = AIController->GetPawn();
    if (!OwnerPawn) return;

    UStudentPerceptorGeertsWarre* SP = Cast<UStudentPerceptorGeertsWarre>(
        OwnerPawn->GetComponentByClass(UStudentPerceptorGeertsWarre::StaticClass()));
    if (!SP) return;

    const TSet<AActor*>& Zones = SP->GetTrackedZones();
    FVector PawnLocation = OwnerPawn->GetActorLocation();

    AActor* NearestDangerZone = nullptr;
    float ClosestDistSq = FMath::Square(DangerRadius);

    for (AActor* Zone : Zones)
    {
        if (!Zone || !IsValid(Zone)) continue;
        float DistSq = FVector::DistSquared(PawnLocation, Zone->GetActorLocation());
        if (DistSq < ClosestDistSq)
        {
            ClosestDistSq = DistSq;
            NearestDangerZone = Zone;
        }
    }

    if (!NearestDangerZone)
    {
        BlackboardComp->SetValueAsBool(InZoneDangerKey.SelectedKeyName, false);
        return;
    }

    // Flee directly away from the zone
    FVector ZoneLocation = NearestDangerZone->GetActorLocation();
    FVector FleeDirection = (PawnLocation - ZoneLocation).GetSafeNormal();
    FVector FleeTarget = PawnLocation + (FleeDirection * FleeDistance);

    BlackboardComp->SetValueAsVector(FleeLocationKey.SelectedKeyName, FleeTarget);
    BlackboardComp->SetValueAsBool(InZoneDangerKey.SelectedKeyName, true);
}