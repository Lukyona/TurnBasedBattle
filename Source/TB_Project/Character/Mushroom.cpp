// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Mushroom.h"
#include "Global.h"
#include "Weapons/EnemyWeapon.h"

AMushroom::AMushroom()
{
    InitializeFromDataTable("Mushroom");

    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -70.f));

    GetCapsuleComponent()->SetCapsuleHalfHeight(69.f);
    GetCapsuleComponent()->SetCapsuleRadius(40.f);
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
