// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Sword_Centaur.h"
#include "Global.h"

ASword_Centaur::ASword_Centaur()
{
    USkeletalMesh* Mesh;
    CHelpers::GetAsset<USkeletalMesh>(&Mesh, "SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Sword.SK_Sword'");
    SkeletalMesh->SetSkeletalMesh(Mesh);

    Capsule->SetRelativeLocation(FVector(0, 0, -75.f));
    Capsule->SetCapsuleHalfHeight(44.f);
    Capsule->SetCapsuleRadius(4.f);
}
