#include "StudentPerceptorGeertsWarre.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"

UStudentPerceptorGeertsWarre::UStudentPerceptorGeertsWarre()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UStudentPerceptorGeertsWarre::BeginPlay()
{
    Super::BeginPlay();

    if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
    {
       PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptorGeertsWarre::OnPerceptionUpdated);
    }
}

void UStudentPerceptorGeertsWarre::UpdateBlackboardData(AActor* Actor, const FAIStimulus& Stimulus)
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return;

    AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
    if (!AIController) return;

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp) return;

    if (ZombieCheck(Actor))
    {
       if (Stimulus.WasSuccessfullySensed())
       {
          TrackedZombies.Add(Actor);
          GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, 
             TEXT("Zombie spotted and added to memory!"));
       }
       else
       {
          TrackedZombies.Remove(Actor);
          GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, 
             TEXT("Zombie lost from direct sight!"));
       }
       
       BlackboardComp->SetValueAsBool(TEXT("HasTrackedZombies"), TrackedZombies.Num() > 0);
    }
    else
    {
       if (Stimulus.WasSuccessfullySensed())
       {
          BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
          BlackboardComp->SetValueAsVector(TEXT("TargetLocation"), Actor->GetActorLocation());
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

bool UStudentPerceptorGeertsWarre::ZombieCheck(AActor* Actor)
{
    if (const APawn* Pawn = Cast<APawn>(Actor))
    {
       if (const AController* Controller = Pawn->GetController())
       {
          if (Controller->GetClass()->GetName().Contains(TEXT("BP_ZombieController")))
          {
             return true;
          }
       }
    }
    return false;
}