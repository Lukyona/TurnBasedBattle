// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"
#include "Character/Enemy.h"


void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());

    if (!enemy) return;

    Speed = enemy->GetCharacterMovement()->GetMaxSpeed();
    Direction = CalculateDirection(enemy->GetVelocity(), enemy->GetControlRotation());
}
