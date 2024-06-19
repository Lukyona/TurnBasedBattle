// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/BTTaskNode_Stop.h"
#include "Global.h"
#include "Character/Enemy.h"
#include "Components/MyMovementComponent.h"
#include "EnemyAI/EnemyController.h"

UBTTaskNode_Stop::UBTTaskNode_Stop()
{
    NodeName = "Stop";
}

EBTNodeResult::Type UBTTaskNode_Stop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AEnemyController* controller = Cast<AEnemyController>(OwnerComp.GetOwner());
    AEnemy* enemy = Cast<AEnemy>(controller->GetPawn());
    CHelpers::GetComponent<UMyMovementComponent>(enemy)->SetStandMode();

    return EBTNodeResult::Succeeded;
}
