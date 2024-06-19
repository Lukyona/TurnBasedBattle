// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/PlayerWeapon.h"
#include "Gun.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API AGun : public APlayerWeapon
{
	GENERATED_BODY()

protected:
	int CurBullet = 10;
	int MaxBullet = 10;
	
	TSubclassOf<class ABullet> BulletClass;
	ABullet* Bullet;

public:
	AGun();

	virtual void SpawnProjectile() override;
	virtual void Shoot() override;

	void Reload();
	int GetCurBulletAmount() { return CurBullet; }


};
