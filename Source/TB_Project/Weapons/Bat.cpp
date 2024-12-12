// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Bat.h"
#include "Global.h"

ABat::ABat()
{
    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Enemies/Goblin/Goblin/Mesh/SM_Weapon_Goblin.SM_Weapon_Goblin'");
    SkeletalMesh->SetSkeletalMesh(mesh);

    Capsule->SetRelativeLocation(FVector(0, 0, 43));
    Capsule->SetCapsuleHalfHeight(28.f);
    Capsule->SetCapsuleRadius(12.f);
}