// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Global.h"
#include "Character/Enemy.h"
#include "Components/HealthComponent.h"

AWeapon_Projectile::AWeapon_Projectile()
{
	CHelpers::CreateComponent(this, &Root, "Root");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Root);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Projectile->bAutoActivate = false;
	Capsule->SetCollisionProfileName("Weapon");
}

void AWeapon_Projectile::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Projectile::OnCapsuleBeginOverlap);
}

void AWeapon_Projectile::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (!Enemy) 
	{
		return;
	}

	UHealthComponent* HealthComp = CHelpers::GetComponent<UHealthComponent>(OtherActor);
	if (!HealthComp->IsAlive()) 
	{
		return;
	}

	FSetElementId ElementId = Hitted.Add(Enemy);
	if (ElementId.IsValidId())
	{
		float Damage = MAINPC->GetSkillDamage();
		UGameplayStatics::ApplyDamage(Enemy, Damage, MAINPC, this, DamageType);
	}

	if (this->IsValidLowLevel()) 
	{
		Destroy();
	}
}

void AWeapon_Projectile::Shoot(FVector Direction)
{
	if (!Direction.IsNormalized())
	{
		Direction.Normalize();
	}

	Projectile->Velocity = Direction * Projectile->InitialSpeed;
	Projectile->Activate();

	//SetLifeSpan(2.f);
}
