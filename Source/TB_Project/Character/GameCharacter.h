// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameCharacter.generated.h"

UCLASS()
class TB_PROJECT_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGameCharacter();

protected:
	virtual void BeginPlay() override;

	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class UMyMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere)
		class UStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere)
		class UTurnComponent* TurnComponent;

	UPROPERTY(VisibleAnywhere)
		class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
		class ACharacter* CombatTarget;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<class ADamageAmount> DamageTextClass;

	ADamageAmount* DamageText;


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Attack() {};
	virtual void EndAttack() {};
	void SetCombatTarget(ACharacter* Target);

	UStateComponent* GetStateComponent();
	UTurnComponent* GetTurnComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCameraWorldLocation();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AAIController* GetAIController() { return AIController; }
};
