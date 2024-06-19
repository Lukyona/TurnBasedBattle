// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayer_Sparrow.h"
#include "Global.h"
#include "Character/PlayerAnimInstance.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerWeaponComponent.h"

ACPlayer_Sparrow::ACPlayer_Sparrow()
{
    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Skins/Rogue/Meshes/Sparrow_Rogue.Sparrow_Rogue'");
    GetMesh()->SetSkeletalMesh(mesh);

    GetMesh()->SetRelativeLocation(FVector(11, 0, -86));

    TSubclassOf<UPlayerAnimInstance> animInstance;
    CHelpers::GetClass<UPlayerAnimInstance>(&animInstance, "AnimBlueprint'/Game/Characters/Sparrow/ABP_Sparrow.ABP_Sparrow_C'");
    GetMesh()->SetAnimClass(animInstance);

    WeaponBoneName = "bow_base";

    GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
    GetCapsuleComponent()->SetCapsuleRadius(34.f);

    MovementComponent->SetWalkSpeed(300.f);
    MovementComponent->SetRunSpeed(600.f);

    TurnComponent->SetOriginMoveingAbility(800.f);

    HealthComponent->SetHealth(20);
    HealthComponent->SetMaxHealth(20);
}

void ACPlayer_Sparrow::BeginPlay()
{
    Super::BeginPlay();
    
    GetMesh()->HideBoneByName("arrow_nock", EPhysBodyOp::PBO_None);

    WeaponComponent->SetCurrentWeapon(EWeaponType::Bow);

}

void ACPlayer_Sparrow::UnEquip()
{
    GetMesh()->HideBoneByName(WeaponBoneName, EPhysBodyOp::PBO_None);

    Super::UnEquip();
}
