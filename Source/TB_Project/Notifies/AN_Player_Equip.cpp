// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_Player_Equip.h"
#include "Character/CPlayer.h"

FString UAN_Player_Equip::GetNotifyName_Implementation() const
{
    return "Player_Equip";
}

void UAN_Player_Equip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner()) return;

    Super::Notify(MeshComp, Animation);

    ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());

    if (player) player->Equip();
}
