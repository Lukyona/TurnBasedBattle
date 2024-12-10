// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/BTTaskNode_Move.h"
#include "NavigationSystem.h"
#include "Global.h"
#include "Character/Enemy.h"
#include "Components/MyMovementComponent.h"
#include "Components/AIBehaviorComponent.h"
#include "EnemyAI/EnemyController.h"

UBTTaskNode_Move::UBTTaskNode_Move()
{
    NodeName = "Move";
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_Move::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AEnemyController* controller = Cast<AEnemyController>(OwnerComp.GetOwner());
    AEnemy* enemy = Cast<AEnemy>(controller->GetPawn());
    UAIBehaviorComponent* behavior = CHelpers::GetComponent<UAIBehaviorComponent>(enemy);
    
    bool combatMode = behavior->GetCombatMode();
    if (combatMode)
    {
        CHelpers::GetComponent<UMyMovementComponent>(enemy)->SetRunMode(); // 전투 중 : 달리기
    }
    else
    {
        CHelpers::GetComponent<UMyMovementComponent>(enemy)->SetWalkMode(); // 평상시 : 걷기
        UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
        FNavLocation patrolLoc(enemy->GetActorLocation());
        bool result = navSystem->GetRandomPointInNavigableRadius(behavior->GetOriginLocation(), Radius, patrolLoc);
        behavior->SetPatrolLocation(patrolLoc.Location);

        if (!result)  return EBTNodeResult::Failed;
    }

    return EBTNodeResult::InProgress;
}


void UBTTaskNode_Move::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AEnemyController* controller = Cast<AEnemyController>(OwnerComp.GetOwner());
    AEnemy* enemy = Cast<AEnemy>(controller->GetPawn());
    UAIBehaviorComponent* behavior = CHelpers::GetComponent<UAIBehaviorComponent>(enemy);

    bool combatMode = behavior->GetCombatMode();
    FVector loc;
    if (combatMode)
        loc = behavior->GetMoveLocation();
    else
        loc = behavior->GetPatrolLocation();

    EPathFollowingRequestResult::Type result = controller->MoveToLocation(loc, 60.f);

    switch (result)
    {
    case EPathFollowingRequestResult::Failed:
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        break;

    case EPathFollowingRequestResult::AlreadyAtGoal:
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        break;
    }

    FVector CurrentLocation = enemy->GetActorLocation();
    if (FVector::DistSquared(LastLocation, CurrentLocation) < FMath::Square(5.0f)) // 5.0f 기준 정체 거리
    {
        StuckTime += DeltaSeconds;
        if (StuckTime > 2.0f) // 2초 동안 정체 상태
        {
            UE_LOG(LogTemp, Warning, TEXT("stppppppp"));
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            return;
        }
    }
    else
    {
        StuckTime = 0.0f; // 정체가 아님
    }

    LastLocation = CurrentLocation; // 위치 갱신
}