// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Centaur.h"
#include "Global.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Weapons/EnemyWeapon.h"

//constexpr const TCHAR* MeshPath = TEXT("SkeletalMesh'/Game/Enemies/Centaur/QuadrapedCreatures/Centaur/Meshes/SK_Centaur.SK_Centaur'");
//constexpr const TCHAR* AnimPath = TEXT("AnimBlueprint'/Game/Enemies/Centaur/ABP_Centaur.ABP_Centaur_C'");
//constexpr const TCHAR* WeaponClassPath = TEXT("Class'/Script/TB_Project.Sword_Centaur'");

ACentaur::ACentaur()
{
    //SetMeshAndAnim(MeshPath, AnimPath);

    GetMesh()->SetRelativeLocation(FVector(-51.f, 0, -99.f));
    GetMesh()->SetRelativeScale3D(FVector(0.7f));

    GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);
    GetCapsuleComponent()->SetCapsuleRadius(30.f);

    MinAttackDistance = 150.f;
    MinDamage = 4.f;
    MaxDamage = 7.f;

    MovementComponent->SetWalkSpeed(300.f);
    MovementComponent->SetRunSpeed(600.f);

    TurnComponent->SetOriginMoveingAbility(500.f);
    TurnComponent->SetOriginActionAbility(1);

    HealthComponent->SetHealth(15);
    HealthComponent->SetMaxHealth(15);

    //CHelpers::GetClass<AEnemyWeapon>(&WeaponClass, WeaponClassPath);
    WeaponSocketName = "SwordAction_Socket";
}

void ACentaur::BeginPlay()
{
    Super::BeginPlay();
}
