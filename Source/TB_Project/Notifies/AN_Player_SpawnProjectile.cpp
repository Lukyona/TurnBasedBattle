// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_Player_SpawnProjectile.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Components/PlayerWeaponComponent.h"

FString UAN_Player_SpawnProjectile::GetNotifyName_Implementation() const
{
    return "Player_SpawnProjectile";
}

void UAN_Player_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner()) 
    {
        return;
    }

    Super::Notify(MeshComp, Animation);

    ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());

    if (Player)
    {
        UPlayerWeaponComponent* WeaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(Player);
        if (WeaponComp) 
        {
            WeaponComp->SpawnProjectile();
        }
    }
}
