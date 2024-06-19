// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayer_Aurora.h"
#include "Global.h"
#include "Character/PlayerAnimInstance.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerWeaponComponent.h"

ACPlayer_Aurora::ACPlayer_Aurora()
{
    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Characters/Aurora/Meshes/Aurora_FrozenHearth.Aurora_FrozenHearth'");
    GetMesh()->SetSkeletalMesh(mesh);

    GetMesh()->SetRelativeLocation(FVector(5, 0, -92));

    TSubclassOf<UPlayerAnimInstance> animInstance;
    CHelpers::GetClass<UPlayerAnimInstance>(&animInstance, "AnimBlueprint'/Game/Characters/Aurora/ABP_Aurora.ABP_Aurora_C'");
    GetMesh()->SetAnimClass(animInstance);

    WeaponBoneName = "weapon";

    GetCapsuleComponent()->SetCapsuleHalfHeight(87.f);
    GetCapsuleComponent()->SetCapsuleRadius(32.f);

    MovementComponent->SetWalkSpeed(350.f);
    MovementComponent->SetRunSpeed(650.f);

    TurnComponent->SetOriginMoveingAbility(1200.f);
    TurnComponent->SetOriginActionAbility(2);

    HealthComponent->SetHealth(28);
    HealthComponent->SetMaxHealth(28);
}

void ACPlayer_Aurora::BeginPlay()
{
    Super::BeginPlay();

    WeaponComponent->SetCurrentWeapon(EWeaponType::Sword);
}