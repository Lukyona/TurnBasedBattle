// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_Player_Reload.h"
#include "Global.h"
#include "Components/PlayerWeaponComponent.h"
#include "Weapons/Gun.h"

FString UAN_Player_Reload::GetNotifyName_Implementation() const
{
    return "Player_Reload";
}

void UAN_Player_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner()) return;

    Super::Notify(MeshComp, Animation);

    UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(MeshComp->GetOwner());
    AGun* gun = Cast<AGun>(weaponComp->GetEquippedWeapon());
    if (gun) gun->Reload();
}
