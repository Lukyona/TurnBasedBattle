// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"
#include "Components/PlayerWeaponComponent.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
    OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
    if (!OwnerCharacter) 
    {
        return;
    }

    UPlayerWeaponComponent* WeaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(OwnerCharacter);
    if(WeaponComp)
    {
        WeaponComp->OnWeaponTypeChanged.AddDynamic(this, &UPlayerAnimInstance::OnWeaponTypeChanged);
    }
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    if (!OwnerCharacter) 
    {
        return;
    }

    Speed = UKismetMathLibrary::VSizeXY(OwnerCharacter->GetVelocity());
    Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());

    UFeetComponent* FeetComp = CHelpers::GetComponent<UFeetComponent>(OwnerCharacter);
    if (FeetComp)
    {
        FeetData = FeetComp->GetData();
    }
}

void UPlayerAnimInstance::OnWeaponTypeChanged(EWeaponType NewType)
{
    WeaponType = NewType;
}
