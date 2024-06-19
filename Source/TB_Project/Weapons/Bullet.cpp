// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Global.h"


ABullet::ABullet()
{
    Capsule->SetRelativeRotation(FRotator(90, 0, 0));
    Capsule->SetCapsuleHalfHeight(3.f);
    Capsule->SetCapsuleRadius(1.f);
    Capsule->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

    CHelpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Capsule);
    UStaticMesh* mesh;
    CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Weapons/Player/Bullet/Meshes/SM_PL45_01g.SM_PL45_01g'");
    StaticMesh->SetStaticMesh(mesh);
    StaticMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

    Projectile->InitialSpeed = 3000.f;
    Projectile->ProjectileGravityScale = 0.f;

}
