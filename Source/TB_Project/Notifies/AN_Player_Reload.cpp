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
    if (!MeshComp || !MeshComp->GetOwner()) 
    {
        return;
    }

    Super::Notify(MeshComp, Animation);

    UPlayerWeaponComponent* WeaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(MeshComp->GetOwner());
    if(WeaponComp)
    {
        AGun* Gun = Cast<AGun>(WeaponComp->GetEquippedWeapon());
        if (Gun)
        {
            Gun->Reload();
        }
    }
}
