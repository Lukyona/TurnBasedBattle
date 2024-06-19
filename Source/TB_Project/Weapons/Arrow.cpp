// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"

AArrow::AArrow()
{
    Capsule->SetRelativeLocation(FVector(0, 0, -64.f));
    Capsule->SetCapsuleHalfHeight(10.f);
    Capsule->SetCapsuleRadius(3.f);

    CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Capsule);
    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapons/Player/Meshes/Bow/Arrrow.Arrrow'");
    SkeletalMesh->SetSkeletalMesh(mesh);
    SkeletalMesh->SetRelativeLocation(FVector(0, 0, 10.f));
    SkeletalMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

    Projectile->InitialSpeed = 1500.f;
    Projectile->ProjectileGravityScale = 0.01f;

}

void AArrow::Shoot(FVector Direction)
{
    bool isUpSkill = MAINPC->GetSelectedSkillName().Contains("Up");

    if (!isUpSkill)
    {
        Super::Shoot(Direction);
        return;
    }

    // 불화살 기술
    Super::Shoot(FVector(0,0,1)); // 위로 쏘기

    ACharacter* target = MAINPC->GetCurPlayer()->GetCombatTarget();
    if (!target) return;
    if (!ArrowSpecialClass) return;

    FVector targetLoc = target->GetActorLocation();
    FTransform transform;
    transform.SetLocation(targetLoc + FVector(0, 0, 500.f));
    GetWorld()->SpawnActor<AArrow>(ArrowSpecialClass, transform);
}
