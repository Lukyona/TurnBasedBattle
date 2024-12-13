// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_Player_UnEquip.h"
#include "Character/CPlayer.h"

FString UAN_Player_UnEquip::GetNotifyName_Implementation() const
{
    return "Player_UnEquip";
}

void UAN_Player_UnEquip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner()) 
    {
        return;
    }

    Super::Notify(MeshComp, Animation);

    ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());

    if (Player) 
    {
        Player->UnEquip();
    }
}
