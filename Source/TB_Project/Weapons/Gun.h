// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/PlayerWeapon.h"
#include "Gun.generated.h"

/**
 * 
 */

class ABullet;

UCLASS()
class TB_PROJECT_API AGun : public APlayerWeapon
{
	GENERATED_BODY()

public:
	AGun();
	virtual void SpawnProjectile() override;
	virtual void Shoot() override;

protected:
	TSubclassOf<ABullet> BulletClass;
	ABullet* Bullet;

	int32 CurBullet = 10;
	int32 MaxBullet = 10;

public:
	int32 GetCurBulletAmount() 
	{
		return CurBullet; 
	}

	void Reload();

};
