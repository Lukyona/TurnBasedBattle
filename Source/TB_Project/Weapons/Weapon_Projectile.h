// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Projectile.generated.h"

class USceneComponent;
class UCapsuleComponent;
class UProjectileMovementComponent;
class ACharacter;

UCLASS()
class TB_PROJECT_API AWeapon_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon_Projectile();
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleDefaultsOnly)
	UProjectileMovementComponent* Projectile;

	UPROPERTY()
	TSet<ACharacter*> Hitted;

	TSubclassOf<UDamageType> DamageType;

protected:
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Shoot(FVector Direction);

};
