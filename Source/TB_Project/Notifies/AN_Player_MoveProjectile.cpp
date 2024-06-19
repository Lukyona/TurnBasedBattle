// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_Player_MoveProjectile.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Components/PlayerWeaponComponent.h"

FString UAN_Player_MoveProjectile::GetNotifyName_Implementation() const
{
    return "Player_MoveProjectile";
}

void UAN_Player_MoveProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner()) return;

    Super::Notify(MeshComp, Animation);

    ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());

    if (player)
    {
        UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(player);
        if (weaponComp) weaponComp->MoveProjectile();
    }
}
