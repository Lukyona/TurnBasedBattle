// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

private:
	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 Health;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 MaxHealth;

public: // Getters and setters
	FORCEINLINE int32 GetHealth() 
	{
		return Health; 
	}
	FORCEINLINE int32 GetMaxHealth() 
	{
		return MaxHealth; 
	}
	void SetMaxHealth(int32 Value) 
	{
		MaxHealth = Value; 
	}
	void SetHealth(int32 Value) 
	{
		Health = Value; 
	}

	void GetDamage(float Damage);

    UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAlive() 
	{
		return Health > 0; 
	}
};
