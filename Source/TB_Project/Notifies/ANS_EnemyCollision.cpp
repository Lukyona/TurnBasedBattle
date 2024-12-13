// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/ANS_EnemyCollision.h"
#include "Global.h"
#include "Character/Enemy.h"
#include "Weapons/EnemyWeapon.h"

FString UANS_EnemyCollision::GetNotifyName_Implementation() const
{
    return "EnemyCollision";
}

void UANS_EnemyCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!MeshComp || !MeshComp->GetOwner()) 
    {
        return;
    }

    Super::NotifyBegin(MeshComp, Animation, TotalDuration);
    
    AEnemy* Enemy = Cast<AEnemy>(MeshComp->GetOwner());
    if (Enemy)
    {
        AEnemyWeapon* Weapon = Enemy->GetWeapon();
        if (Weapon) 
        {
            Weapon->OnCollision();
        }

        if (bIsDoubleWeapon)
        {
            Weapon = Enemy->GetSecondWeapon();
            if (Weapon) 
            {
                Weapon->OnCollision();
            }
        }
    }
}

void UANS_EnemyCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner()) 
    {
        return;
    }

    Super::NotifyEnd(MeshComp, Animation);

    AEnemy* Enemy = Cast<AEnemy>(MeshComp->GetOwner());
    if (Enemy)
    {
        AEnemyWeapon* Weapon = Enemy->GetWeapon();
        if (Weapon) 
        {
            Weapon->OffCollision();
        }

        if (bIsDoubleWeapon)
        {
            Weapon = Enemy->GetSecondWeapon();
            if (Weapon) 
            {
                Weapon->OffCollision();
            }
        }
    }
}
