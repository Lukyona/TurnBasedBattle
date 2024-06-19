// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon_Projectile.h"
#include "Bullet.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API ABullet : public AWeapon_Projectile
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

public:
	ABullet();


};
