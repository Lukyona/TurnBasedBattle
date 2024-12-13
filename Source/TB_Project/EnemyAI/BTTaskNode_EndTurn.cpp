// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/BTTaskNode_EndTurn.h"
#include "Global.h"
#include "Character/Enemy.h"
#include "Components/MyMovementComponent.h"
#include "Components/StateComponent.h"
#include "Components/TurnComponent.h"
#include "Components/AIBehaviorComponent.h"
#include "EnemyAI/EnemyController.h"

UBTTaskNode_EndTurn::UBTTaskNode_EndTurn()
{
    NodeName = "EndTurn";
}

EBTNodeResult::Type UBTTaskNode_EndTurn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AEnemyController* EnemyController;
    AEnemy* Enemy;
    UAIBehaviorComponent* BehaviorComp;
    UMyMovementComponent* MovementComp;
    UStateComponent* StateComp;
    UTurnComponent* TurnComp;

    bool bIsValid = BTHelper::ValidateAllEntities(OwnerComp, EnemyController, Enemy, &BehaviorComp, &MovementComp, &StateComp, &TurnComp);
    if (!bIsValid)
    {
        return EBTNodeResult::Failed;
    }

    MovementComp->SetInterp(false);
    StateComp->SetWaitMode();

    int32 TurnNum = TurnComp->GetTurnNum();
    if(MAINPC)
    {
        MAINPC->StartTurn(TurnNum);
    }

    BehaviorComp->SetTurn(false);

    return EBTNodeResult::Succeeded;
}
