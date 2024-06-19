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

    AEnemyController* controller = Cast<AEnemyController>(OwnerComp.GetOwner());
    AEnemy* enemy = Cast<AEnemy>(controller->GetPawn());
    UAIBehaviorComponent* behavior = CHelpers::GetComponent<UAIBehaviorComponent>(enemy);

    CHelpers::GetComponent<UMyMovementComponent>(enemy)->SetInterp(false);
    CHelpers::GetComponent<UStateComponent>(enemy)->SetWaitMode();
    int turnNum = CHelpers::GetComponent<UTurnComponent>(enemy)->GetTurnNum();

    MAINPC->StartTurn(turnNum);

    behavior->SetTurn(false);

    return EBTNodeResult::Succeeded;
}