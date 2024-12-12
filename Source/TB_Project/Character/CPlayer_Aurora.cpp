// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayer_Aurora.h"
#include "Global.h"
#include "Character/PlayerAnimInstance.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerWeaponComponent.h"

static const FName AuroraKey(TEXT("Aurora"));

ACPlayer_Aurora::ACPlayer_Aurora()
{
    InitializeFromDataTable(AuroraKey);
    SetTransform(FVector(5, 0, -92));
    SetCapsuleSize(87.f, 32.f);
}

void ACPlayer_Aurora::BeginPlay()
{
    Super::BeginPlay();
    if (WeaponComponent)
    {
        WeaponComponent->SetCurrentWeapon(EWeaponType::Sword);
    }
}