// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameCharacter.generated.h"

class AAIController;
class USpringArmComponent;
class UCameraComponent;
class UMyMovementComponent;
class UStateComponent;
class UTurnComponent;
class UHealthComponent;
class ACharacter;
class ADamageAmount;

UCLASS()
class TB_PROJECT_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGameCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Attack() {};
public:
	virtual void EndAttack() {};

protected:
	UPROPERTY()
	AAIController* AIController;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	UMyMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere)
	UStateComponent* StateComponent;
	UPROPERTY(VisibleAnywhere)
	UTurnComponent* TurnComponent;
	UPROPERTY(VisibleAnywhere)
	UHealthComponent* HealthComponent;

	UPROPERTY()
	ACharacter* CombatTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<ADamageAmount> DamageTextClass;

	UPROPERTY()
	ADamageAmount* DamageText;

public:	//Getters and setters
	UStateComponent* GetStateComponent();
	UTurnComponent* GetTurnComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCameraWorldLocation();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AAIController* GetAIController() 
	{
		return AIController; 
	}

public://Core methods
	void SetCombatTarget(ACharacter* Target)
	{
		CombatTarget = Target;
	}

protected:
	void SetMeshAndAnim(const FString& MeshPath, const FString& AnimClassPath);
	void SetTransform(const FVector& InTranslation = FVector::ZeroVector, const FVector& InScale3D = FVector::OneVector);
	void SetCapsuleSize(float HalfHeight, float Radius = 34.f);

};
