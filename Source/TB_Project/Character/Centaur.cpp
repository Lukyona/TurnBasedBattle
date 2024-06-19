// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Centaur.h"
#include "Global.h"
#include "Character/EnemyAnimInstance.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Weapons/EnemyWeapon.h"

ACentaur::ACentaur()
{
    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Enemies/Centaur/QuadrapedCreatures/Centaur/Meshes/SK_Centaur.SK_Centaur'");
    GetMesh()->SetSkeletalMesh(mesh);

    TSubclassOf<UEnemyAnimInstance> animInstance;
    CHelpers::GetClass<UEnemyAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemies/Centaur/ABP_Centaur.ABP_Centaur_C'");
    GetMesh()->SetAnimClass(animInstance);

    GetMesh()->SetRelativeLocation(FVector(-51.f, 0, -99.f));
    GetMesh()->SetRelativeScale3D(FVector(0.7f));

    GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);
    GetCapsuleComponent()->SetCapsuleRadius(30.f);

    AttackDistance = 140.f;
    MinDamage = 4.f;
    MaxDamage = 7.f;

    MovementComponent->SetWalkSpeed(300.f);
    MovementComponent->SetRunSpeed(600.f);

    TurnComponent->SetOriginMoveingAbility(500.f);
    TurnComponent->SetOriginActionAbility(1);

    HealthComponent->SetHealth(15);
    HealthComponent->SetMaxHealth(15);

    CHelpers::GetClass<AEnemyWeapon>(&WeaponClass, "Class'/Script/TB_Project.Sword_Centaur'");
    WeaponSocketName = "SwordAction_Socket";

    Tags.Add("Area3");

}

void ACentaur::BeginPlay()
{
    Super::BeginPlay();

}
