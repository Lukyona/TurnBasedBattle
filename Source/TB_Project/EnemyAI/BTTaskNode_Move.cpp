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

    AEnemyController* EnemyController;
    AEnemy* Enemy;
    UAIBehaviorComponent* BehaviorComp;
    UMyMovementComponent* MovementComp;
    bool bIsValid = BTHelper::ValidateAllEntities(OwnerComp, EnemyController, Enemy, &BehaviorComp, &MovementComp);
    if (!bIsValid)
    {
        return EBTNodeResult::Failed;
    }

    bool bIsCombatMode = BehaviorComp->GetCombatMode();
    if (bIsCombatMode)
    {
        MovementComp->SetRunMode(); // 전투 중 : 달리기
    }
    else 
    {// 순찰 상태
        MovementComp->SetWalkMode(); // 평상시 : 걷기
        UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
        FNavLocation PatrolLoc(Enemy->GetActorLocation());
        bool bCanNavigate = NavSystem->GetRandomPointInNavigableRadius(BehaviorComp->GetOriginLocation(), Radius, PatrolLoc);
        if (bCanNavigate)
        {
            BehaviorComp->SetPatrolLocation(PatrolLoc.Location);
        }
    }

    return EBTNodeResult::InProgress;
}

void UBTTaskNode_Move::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AEnemyController* EnemyController;
    AEnemy* Enemy;
    UAIBehaviorComponent* BehaviorComp;
    bool bIsValid = BTHelper::ValidateAllEntities(OwnerComp, EnemyController, Enemy, &BehaviorComp);
    if (!bIsValid)
    {
        return;
    }

    bool bIsCombatMode = BehaviorComp->GetCombatMode();
    FVector TargetLoc;
    if (bIsCombatMode)
    {
        TargetLoc = BehaviorComp->GetMoveLocation();
    }
    else
    {
        TargetLoc = BehaviorComp->GetPatrolLocation();
    }

    EPathFollowingRequestResult::Type MoveResult = EnemyController->MoveToLocation(TargetLoc, 60.f);
    switch (MoveResult)
    {
        case EPathFollowingRequestResult::Failed:
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            break;
        case EPathFollowingRequestResult::AlreadyAtGoal:
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            break;
        default:
            break;
    }

    bool bIsStuck = CheckStuckState(DeltaSeconds, Enemy);
    if (bIsStuck)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s stuck"), *Enemy->GetName());
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }
}

bool UBTTaskNode_Move::CheckStuckState(float DeltaSeconds, AEnemy* InEnemy)
{
    FVector CurrentLocation = InEnemy->GetActorLocation();
    if (FVector::DistSquared(LastLocation, CurrentLocation) < FMath::Square(5.0f))
    {
        StuckTime += DeltaSeconds;
        return (StuckTime > 1.5f); // 정체 상태
    }
    else
    {
        StuckTime = 0.0f;
    }

    LastLocation = CurrentLocation;
    return false; // 정체 아님
}
