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

    AEnemyController* EnemyController;
    AEnemy* Enemy;
    UAIBehaviorComponent* BehaviorComp;

    bool bIsValid = BTHelper::ValidateAllEntities(OwnerComp, EnemyController, Enemy, &BehaviorComp);
    if (!bIsValid)
    {
        return EBTNodeResult::Failed;
    }

    ACharacter* Target = BehaviorComp->GetTarget();
    if(!Target) 
    {
        return EBTNodeResult::Failed;
    }

    float AttckDistance = Enemy->GetMinAttackDistance();
    float TargetDistance = UKismetMathLibrary::Vector_Distance(Target->GetActorLocation(), Enemy->GetActorLocation());

    if (TargetDistance < AttckDistance)
    {
        BehaviorComp->SetCanAttack(true);
        return EBTNodeResult::Succeeded;
    }

    float AvailableMoveDistance = CHelpers::GetComponent<UTurnComponent>(Enemy)->GetOriginMovingAbility();
    BehaviorComp->SetMoveDistance(AvailableMoveDistance);
    BehaviorComp->SetCanAttack(false);

    return EBTNodeResult::Succeeded;
}
