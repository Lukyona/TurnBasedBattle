// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon_Projectile.h"
#include "Arrow.generated.h"

/**
 * 
 */
class USkeletalMeshComponent;
class AArrow;

UCLASS()
class TB_PROJECT_API AArrow : public AWeapon_Projectile
{
	GENERATED_BODY()

public:
	AArrow();

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AArrow> ArrowSpecialClass;

public:
	virtual void Shoot(FVector Direction);

};
