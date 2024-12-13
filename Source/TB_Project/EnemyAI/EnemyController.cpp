// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAI/EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Global.h"
#include "Character/Enemy.h"
#include "Components/AIBehaviorComponent.h"
#include "Components/MyMovementComponent.h"
#include "Components/StateComponent.h"
#include "Components/TurnComponent.h"

AEnemyController::AEnemyController()
{
    CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "BlackBoard");
}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();

    AEnemy* Enemy = Cast<AEnemy>(GetPawn());

    if (!Enemy->GetBehaviorTree()) 
    {
        return;
    }

    UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);
    Behavior = CHelpers::GetComponent<UAIBehaviorComponent>(Enemy);
    if (!Behavior) 
    {
        return;
    }

    Behavior->SetBlackboard(Blackboard);
    Blackboard->SetValueAsVector("OriginLocation", Enemy->GetActorLocation());
    Blackboard->SetValueAsVector("PatrolLocation", FVector(-450.f, 1760.f, -550.f));

    RunBehaviorTree(Enemy->GetBehaviorTree());
}

bool BTHelper::ValidateAllEntities(UBehaviorTreeComponent& OwnerComp, AEnemyController*& OutEnemyController, AEnemy*& OutEnemy, UAIBehaviorComponent** OutBehaviorComp, UMyMovementComponent** OutMovementComp, UStateComponent** OutStateComp, UTurnComponent** OutTurnComp)
{
    OutEnemyController = Cast<AEnemyController>(OwnerComp.GetOwner());
    if (!OutEnemyController)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyController is null."));
        return false;
    }

    OutEnemy = Cast<AEnemy>(OutEnemyController->GetPawn());
    if (!OutEnemy)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy is null."));
        return false;
    }

    if (OutBehaviorComp)
    {
        *OutBehaviorComp = CHelpers::GetComponent<UAIBehaviorComponent>(OutEnemy);
        if (!*OutBehaviorComp)
        {
            UE_LOG(LogTemp, Warning, TEXT("MovementComp is missing."));
            return false;
        }
    }

    if (OutMovementComp)
    {
        *OutMovementComp = CHelpers::GetComponent<UMyMovementComponent>(OutEnemy);
        if (!*OutMovementComp)
        {
            UE_LOG(LogTemp, Warning, TEXT("MovementComp is missing."));
            return false;
        }
    }

    if (OutStateComp)
    {
        *OutStateComp = CHelpers::GetComponent<UStateComponent>(OutEnemy);
        if (!*OutStateComp)
        {
            UE_LOG(LogTemp, Warning, TEXT("StateComp is missing."));
            return false;
        }
    }

    if (OutTurnComp)
    {
        *OutTurnComp = CHelpers::GetComponent<UTurnComponent>(OutEnemy);
        if (!*OutTurnComp)
        {
            UE_LOG(LogTemp, Warning, TEXT("TurnComp is missing."));
            return false;
        }
    }

    return true;
}
