// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayer_Revenant.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"

#include "Global.h"
#include "Character/PlayerAnimInstance.h"
#include "Components/MyMovementComponent.h"
#include "Components/TurnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerWeaponComponent.h"
#include "Components/FeetComponent.h"

static const FName RevenantKey(TEXT("Revenant"));

ACPlayer_Revenant::ACPlayer_Revenant()
{
    InitializeFromDataTable(RevenantKey);

    SetTransform(FVector(10, 0, -98), FVector(0.9f));
    SetCapsuleSize(94.f, 37.f);
}

void ACPlayer_Revenant::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* MyController = Cast<APlayerController>(GetController());
    if(MyController)
    {
        AMainPlayerController::SetMainPC(MyController);
    }

    FTransform Transform;
    AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, Transform);

    if(MAINPC)
    {
        MAINPC->StartFollowingPlayer();
    }

    if (WeaponComponent)
    {
        WeaponComponent->SetCurrentWeapon(EWeaponType::Gun);
        WeaponComponent->SetSkillIcons();
    }
}
