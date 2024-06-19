// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_Enemy_EndDead.h"
#include "Character/Enemy.h"

FString UAN_Enemy_EndDead::GetNotifyName_Implementation() const
{
    return "Enemy_EndDead";
}

void UAN_Enemy_EndDead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner()) return;

    Super::Notify(MeshComp, Animation);

    AEnemy* enemy = Cast<AEnemy>(MeshComp->GetOwner());

    if (enemy) enemy->EndDead();
}
