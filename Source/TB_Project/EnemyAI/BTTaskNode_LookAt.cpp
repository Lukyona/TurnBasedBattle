// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/BTTaskNode_LookAt.h"
#include "Global.h"
#include "Character/Enemy.h"
#include "Components/MyMovementComponent.h"
#include "Components/AIBehaviorComponent.h"
#include "EnemyAI/EnemyController.h"

UBTTaskNode_LookAt::UBTTaskNode_LookAt()
{
    NodeName = "LookAt";
}

EBTNodeResult::Type UBTTaskNode_LookAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    EnemyController->StopMovement();

    //가장 가까운 상대를 찾아 타겟으로 지정
    ACharacter* Target = MAINPC ? MAINPC->FindNearestTarget(Enemy) : nullptr;
    if (!Target)
    {
        return EBTNodeResult::Failed;
    }

    BehaviorComp->SetTarget(Target);
    Enemy->SetCombatTarget(Target);
    MovementComp->SetInterp(true);

    return EBTNodeResult::Succeeded;
}
