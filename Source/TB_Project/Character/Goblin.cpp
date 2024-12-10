// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Goblin.h"
#include "Global.h"
#include "Character/EnemyAnimInstance.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Weapons/EnemyWeapon.h"

AGoblin::AGoblin()
{
    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Enemies/Goblin/Goblin/Mesh/SK_Goblin.SK_Goblin'");
    GetMesh()->SetSkeletalMesh(mesh);

    TSubclassOf<UEnemyAnimInstance> animInstance;
    CHelpers::GetClass<UEnemyAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemies/Goblin/ABP_Goblin.ABP_Goblin_C'");
    GetMesh()->SetAnimClass(animInstance);

    GetMesh()->SetRelativeLocation(FVector(0, 0, -79.f));
    GetMesh()->SetRelativeScale3D(FVector(1.2f));

    GetCapsuleComponent()->SetCapsuleHalfHeight(80.f);

    MinAttackDistance = 140.f;
    MinDamage = 2.f;
    MaxDamage = 5.f;

    MovementComponent->SetWalkSpeed(175.f);
    MovementComponent->SetRunSpeed(300.f);

    TurnComponent->SetOriginMoveingAbility(400.f);
    TurnComponent->SetOriginActionAbility(1);

    HealthComponent->SetHealth(10);
    HealthComponent->SetMaxHealth(10);
    CHelpers::GetClass<AEnemyWeapon>(&WeaponClass, "Class'/Script/TB_Project.Bat'");
    WeaponSocketName = "skt_weapon";

}

void AGoblin::BeginPlay()
{
    Super::BeginPlay();
}
