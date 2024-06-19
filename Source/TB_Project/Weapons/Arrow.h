// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon_Projectile.h"
#include "Arrow.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API AArrow : public AWeapon_Projectile
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* SkeletalMesh;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AArrow> ArrowSpecialClass;

public:
	AArrow();

	virtual void Shoot(FVector Direction);

};
