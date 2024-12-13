// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AN_EndAttack.h"
#include "Character/GameCharacter.h"

FString UAN_EndAttack::GetNotifyName_Implementation() const
{
    return "EndAttack";
}

void UAN_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !MeshComp->GetOwner()) 
    {
        return;
    }

    Super::Notify(MeshComp, Animation);

    AGameCharacter* Character = Cast<AGameCharacter>(MeshComp->GetOwner());
    if (Character) 
    {
        Character->EndAttack();
    }
}
