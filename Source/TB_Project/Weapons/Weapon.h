// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USceneComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class ACharacter;

UCLASS()
class TB_PROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly)
	UCapsuleComponent* Capsule;

	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	TSet<ACharacter*> Hitted;

protected:
	ACharacter* GetOwnerCharacter();

	virtual void ApplyDamageToTarget(ACharacter* OtherCharacter) {};

public:	
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnCollision();
	void OffCollision();
};
