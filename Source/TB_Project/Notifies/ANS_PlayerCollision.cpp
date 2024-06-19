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
    if (!MeshComp || !MeshComp->GetOwner()) return;

    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
    if (player)
    {
        UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(player);
        if (weaponComp)
        {
            APlayerWeapon* weapon = weaponComp->GetEquippedWeapon();
            if (weapon) weapon->OnCollision();
        }
    }
}

void UANS_PlayerCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner()) return;

    Super::NotifyEnd(MeshComp, Animation);

    ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
    if (player)
    {
        UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(player);
        if (weaponComp)
        {
            APlayerWeapon* weapon = weaponComp->GetEquippedWeapon();
            if (weapon) weapon->OffCollision();
        }
    }
}
