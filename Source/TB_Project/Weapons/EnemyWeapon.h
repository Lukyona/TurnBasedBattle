// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "EnemyWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TB_PROJECT_API AEnemyWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AEnemyWeapon();

	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void ApplyDamageToTarget(ACharacter* OtherCharacter) override;



};
