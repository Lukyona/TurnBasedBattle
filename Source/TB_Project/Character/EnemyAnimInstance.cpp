// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"
#include "Character/Enemy.h"

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    AEnemy* Enemy = Cast<AEnemy>(TryGetPawnOwner());

    if (!Enemy) 
    {
        return;
    }

    Speed = Enemy->GetCharacterMovement()->GetMaxSpeed();
    Direction = CalculateDirection(Enemy->GetVelocity(), Enemy->GetControlRotation());
}
