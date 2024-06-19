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

    AEnemyController* controller = Cast<AEnemyController>(OwnerComp.GetOwner());
    AEnemy* enemy = Cast<AEnemy>(controller->GetPawn());
    UAIBehaviorComponent* behavior = CHelpers::GetComponent<UAIBehaviorComponent>(enemy);

    controller->StopMovement();

    ACharacter* target = MAINPC->FindNearestTarget(enemy);
    behavior->SetTarget(target);

    enemy->SetCombatTarget(target);

    CHelpers::GetComponent<UMyMovementComponent>(enemy)->SetInterp(true);

    return EBTNodeResult::Succeeded;
}

