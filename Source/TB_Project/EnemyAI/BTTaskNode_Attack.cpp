// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/BTTaskNode_Attack.h"
#include "Global.h"
#include "Character/Enemy.h"
#include "EnemyAI/EnemyController.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
    NodeName = "Attack";
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AEnemyController* controller = Cast<AEnemyController>(OwnerComp.GetOwner());
    AEnemy* enemy = Cast<AEnemy>(controller->GetPawn());

    enemy->Attack();

    return EBTNodeResult::Succeeded;
}
