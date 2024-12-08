// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayer_Revenant.h"
#include "GameFramework/PlayerController.h"
#include "Global.h"
#include "Character/PlayerAnimInstance.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerWeaponComponent.h"
#include "Components/FeetComponent.h"
#include "AIController.h"


ACPlayer_Revenant::ACPlayer_Revenant()
{
    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Characters/ParagonRevenant/Characters/Heroes/Revenant/Meshes/Revenant.Revenant'");
    GetMesh()->SetSkeletalMesh(mesh);

    GetMesh()->SetRelativeLocation(FVector(10, 0, -98));
    GetMesh()->SetRelativeScale3D(FVector(0.9f));

    TSubclassOf<UPlayerAnimInstance> animInstance;
    CHelpers::GetClass<UPlayerAnimInstance>(&animInstance, "AnimBlueprint'/Game/Characters/Revenant/ABP_Revenent.ABP_Revenent_C'");
    GetMesh()->SetAnimClass(animInstance);

    WeaponBoneName = "weapon_l";

    GetCapsuleComponent()->SetCapsuleHalfHeight(94.f);
    GetCapsuleComponent()->SetCapsuleRadius(34.f);

    MovementComponent->SetWalkSpeed(200.f);
    MovementComponent->SetRunSpeed(550.f);

    TurnComponent->SetOriginMoveingAbility(1000.f);

    HealthComponent->SetHealth(25);
    HealthComponent->SetMaxHealth(25);

}

void ACPlayer_Revenant::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* controller = Cast<APlayerController>(GetController());
    if(controller)
        AMainPlayerController::SetMainPC(controller);

    FTransform transform;
    AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, transform);

    MAINPC->StartFollowingPlayer();

    WeaponComponent->SetCurrentWeapon(EWeaponType::Gun);
    WeaponComponent->SetSkillIcons();

}
