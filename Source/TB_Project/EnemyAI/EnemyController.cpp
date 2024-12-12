// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Global.h"
#include "Character/Enemy.h"
#include "Components/AIBehaviorComponent.h"


AEnemyController::AEnemyController()
{
    CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "BlackBoard");
    
}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();

    AEnemy* enemy = Cast<AEnemy>(GetPawn());

    if (!enemy->GetBehaviorTree()) return;

    UseBlackboard(enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);
    Behavior = CHelpers::GetComponent<UAIBehaviorComponent>(enemy);
    if (!Behavior) return;

    Behavior->SetBlackboard(Blackboard);
    Blackboard->SetValueAsVector("OriginLocation", enemy->GetActorLocation());
    Blackboard->SetValueAsVector("PatrolLocation", FVector(-450.f, 1760.f, -550.f));


    RunBehaviorTree(enemy->GetBehaviorTree());


}
