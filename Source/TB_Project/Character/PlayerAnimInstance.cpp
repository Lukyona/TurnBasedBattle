// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"
#include "Components/PlayerWeaponComponent.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
    OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
    if (!OwnerCharacter) return;

    UPlayerWeaponComponent* weaponComp = CHelpers::GetComponent<UPlayerWeaponComponent>(OwnerCharacter);
    weaponComp->OnWeaponTypeChanged.AddDynamic(this, &UPlayerAnimInstance::OnWeaponTypeChanged);
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    if (!OwnerCharacter) return;

    Speed = UKismetMathLibrary::VSizeXY(OwnerCharacter->GetVelocity());
    Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());

    UFeetComponent* feet = CHelpers::GetComponent<UFeetComponent>(OwnerCharacter);

    if (feet)
        FeetData = feet->GetData();

}

void UPlayerAnimInstance::OnWeaponTypeChanged(EWeaponType NewType)
{
    WeaponType = NewType;
}
