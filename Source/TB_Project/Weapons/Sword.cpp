// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Sword.h"
#include "Global.h"

ASword::ASword()
{
    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapons/Player/Meshes/Sword/Sword.Sword'");
    SkeletalMesh->SetSkeletalMesh(mesh);

    Capsule->SetRelativeLocation(FVector(0, 50, 0));
    Capsule->SetRelativeRotation(FRotator(0, 0, 90));
    Capsule->SetCapsuleHalfHeight(30.f);
    Capsule->SetCapsuleRadius(4.f);
    Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
    AttackDistance = 160.f;

}