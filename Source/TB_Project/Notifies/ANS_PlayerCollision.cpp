// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/ANS_PlayerCollision.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Components/PlayerWeaponComponent.h"
#include "Weapons/PlayerWeapon.h"

FString UANS_PlayerCollision::GetNotifyName_Implementation() const
{
    return "PlayerCollision";
}

void UANS_PlayerCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!MeshComp || !MeshComp->GetOwner()) 
    {
        return;
    }

    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
    if (Player)
    {
        UPlayerWeaponComponent* WeaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(Player);
        if (WeaponComp)
        {
            APlayerWeapon* Weapon = WeaponComp->GetEquippedWeapon();
            if (Weapon) 
            {
                Weapon->OnCollision();
            }
        }
    }
}

void UANS_PlayerCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner()) 
    {
        return;
    }

    Super::NotifyEnd(MeshComp, Animation);

    ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
    if (Player)
    {
        UPlayerWeaponComponent* WeaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(Player);
        if (WeaponComp)
        {
            APlayerWeapon* Weapon = WeaponComp->GetEquippedWeapon();
            if (Weapon) 
            {
                Weapon->OffCollision();
            }
        }
    }
}
