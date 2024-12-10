// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/BTTaskNode_CalculateDistance.h"
#include "Global.h"
#include "Character/Enemy.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/AIBehaviorComponent.h"
#include "EnemyAI/EnemyController.h"

UBTTaskNode_CalculateDistance::UBTTaskNode_CalculateDistance()
{
    NodeName = "CalculateDistance";
}

EBTNodeResult::Type UBTTaskNode_CalculateDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AEnemyController* controller = Cast<AEnemyController>(OwnerComp.GetOwner());
    AEnemy* enemy = Cast<AEnemy>(controller->GetPawn());
    UAIBehaviorComponent* behavior = CHelpers::GetComponent<UAIBehaviorComponent>(enemy);

    ACharacter* target = behavior->GetTarget();
    if(!target) return EBTNodeResult::Failed;

    float attckDist = enemy->GetMinAttackDistance();
    float dist = UKismetMathLibrary::Vector_Distance(target->GetActorLocation(), enemy->GetActorLocation());

    if (dist < attckDist)
    {
        behavior->SetCanAttack(true);
        return EBTNodeResult::Succeeded;
    }

    float moveDist = CHelpers::GetComponent<UTurnComponent>(enemy)->GetOriginMovingAbility();
    behavior->SetMoveDistance(moveDist);
    behavior->SetCanAttack(false);

    return EBTNodeResult::Succeeded;
}

