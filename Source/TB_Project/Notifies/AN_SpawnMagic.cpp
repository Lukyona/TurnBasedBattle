// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_SpawnMagic.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Components/PlayerWeaponComponent.h"

FString UAN_SpawnMagic::GetNotifyName_Implementation() const
{
    return "Player_SpawnMagic";
}

void UAN_SpawnMagic::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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
            WeaponComp->SpawnMagic(Type);
        }
    }
}
