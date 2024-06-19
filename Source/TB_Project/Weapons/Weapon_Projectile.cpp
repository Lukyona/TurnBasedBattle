// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon_Projectile.h"
#include "Global.h"
#include "Character/Enemy.h"
#include "Components/HealthComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AWeapon_Projectile::AWeapon_Projectile()
{
	CHelpers::CreateComponent(this, &Root, "Root");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Root);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Projectile->bAutoActivate = false;
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);

}

void AWeapon_Projectile::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Projectile::OnCapsuleBeginOverlap);

}

void AWeapon_Projectile::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* enemy = Cast<AEnemy>(OtherActor);
	if (!enemy) return;

	UHealthComponent* healthComp = CHelpers::GetComponent<UHealthComponent>(OtherActor);
	if (!healthComp->IsAlive()) return;

	if (Hitted.Find(enemy) != INDEX_NONE) return;
	
	Hitted.AddUnique(enemy);
	float damage = MAINPC->GetSkillDamage();
	UGameplayStatics::ApplyDamage(enemy, damage, MAINPC, this, DamageType);

	if (this->IsValidLowLevel()) Destroy();
}

void AWeapon_Projectile::Shoot(FVector Direction)
{
	Projectile->Velocity = Direction * Projectile->InitialSpeed;
	Projectile->Activate();

	SetLifeSpan(2.f);
}


