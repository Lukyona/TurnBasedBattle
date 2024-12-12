// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayer_Sparrow.h"
#include "Global.h"
#include "Character/PlayerAnimInstance.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerWeaponComponent.h"

static const FName SparrowKey(TEXT("Sparrow"));
ACPlayer_Sparrow::ACPlayer_Sparrow()
{
    InitializeFromDataTable(SparrowKey);
    SetTransform(FVector(11, 0, -86));
    SetCapsuleSize(90.f);
}

void ACPlayer_Sparrow::BeginPlay()
{
    Super::BeginPlay();
    
    GetMesh()->HideBoneByName("arrow_nock", EPhysBodyOp::PBO_None);
    if(WeaponComponent)
    {
        WeaponComponent->SetCurrentWeapon(EWeaponType::Bow);
    }
}

void ACPlayer_Sparrow::UnEquip()
{
    GetMesh()->HideBoneByName(WeaponBoneName, EPhysBodyOp::PBO_None);

    Super::UnEquip();
}
