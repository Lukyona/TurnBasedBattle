// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Health")
	int Health;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int MaxHealth;


public:
	UHealthComponent();

	void SetMaxHealth(int Value) { MaxHealth = Value; }
	void SetHealth(int Value) { Health = Value; }

	FORCEINLINE int GetHealth() { return Health; }
	FORCEINLINE int GetMaxHealth() { return MaxHealth; }

	void GetDamage(float Damage);

    UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAlive() { return Health > 0 ? true : false; }
};
