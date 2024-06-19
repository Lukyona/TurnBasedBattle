// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Gun.h"
#include "Components/CapsuleComponent.h"
#include "Global.h"
#include "Weapons/Bullet.h"

AGun::AGun()
{
    USkeletalMesh* mesh;
    CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapons/Player/Meshes/Gun/Gun.Gun'");
    SkeletalMesh->SetSkeletalMesh(mesh);

    AttackDistance = 1500.f;
    Capsule->SetRelativeScale3D(FVector::ZeroVector);
    Capsule->SetGenerateOverlapEvents(false);

    CHelpers::GetClass<ABullet>(&BulletClass, "Blueprint'/Game/Weapons/Player/BP_Bullet.BP_Bullet_C'");

}

void AGun::SpawnProjectile()
{
    FTransform transform = SkeletalMesh->GetSocketTransform("BulletSocket");
    Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, transform);
    if (!Bullet) return;
    Bullet->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, "BulletSocket");
}

void AGun::Shoot()
{
    if (!Bullet) return;

    Bullet->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
    FVector dir = GetOwnerCharacter()->GetCapsuleComponent()->GetForwardVector();
    Bullet->Shoot(dir);

    --CurBullet;

    MAINPC->UpdateBulletText(CurBullet, MaxBullet);
}

void AGun::Reload()
{
    CurBullet = MaxBullet;
    MAINPC->UpdateBulletText(CurBullet, MaxBullet);
}
