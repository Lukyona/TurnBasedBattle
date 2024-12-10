// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Mushroom.h"
#include "Global.h"
#include "Character/EnemyAnimInstance.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Weapons/EnemyWeapon.h"

AMushroom::AMushroom()
{
    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Enemies/Mushroom/Fungus_Creature/Mesh/SK_Fungus.SK_Fungus'");
    GetMesh()->SetSkeletalMesh(mesh);

    TSubclassOf<UEnemyAnimInstance> animInstance;
    CHelpers::GetClass<UEnemyAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemies/Mushroom/ABP_Mushroom.ABP_Mushroom_C'");
    GetMesh()->SetAnimClass(animInstance);

    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -70.f));

    GetCapsuleComponent()->SetCapsuleHalfHeight(69.f);
    GetCapsuleComponent()->SetCapsuleRadius(40.f);

    MinAttackDistance = 150.f;
    MinDamage = 2.f;
    MaxDamage = 3.f;

    MovementComponent->SetWalkSpeed(150.f);
    MovementComponent->SetRunSpeed(300.f);

    TurnComponent->SetOriginMoveingAbility(400.f);
    TurnComponent->SetOriginActionAbility(1);

    HealthComponent->SetHealth(12);
    HealthComponent->SetMaxHealth(12);

    CHelpers::GetClass<AEnemyWeapon>(&WeaponClass, "Class'/Script/TB_Project.Fist_Enemy'");
    WeaponSocketName = "Fist1";
}

void AMushroom::BeginPlay()
{
    Super::BeginPlay();

    FTransform transform;
    FActorSpawnParameters params;
    params.Owner = this;
    SecondWeapon = GetWorld()->SpawnActor<AEnemyWeapon>(WeaponClass, transform, params);

    WeaponSocketName = "Fist2";
    SecondWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketName);
}
