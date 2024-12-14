// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "DamageAmount.generated.h"

class UDamageAmountText;
class UWidgetComponent;
class UTimelineComponent;

UCLASS()
class TB_PROJECT_API ADamageAmount : public AActor
{
	GENERATED_BODY()

public:
	ADamageAmount();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	UDamageAmountText* DamageText;

    UPROPERTY(VisibleAnywhere)
	UWidgetComponent* Widget;

	FVector StartLoc, EndLoc;

	UPROPERTY()
	UTimelineComponent* Timeline;

	FVector PreviousCameraLoc;

protected:
	int32 Damage;

	UPROPERTY()
	UCurveFloat* Curve;

public:	//Getter and setter
	int32 GetDamage() 
	{
		return Damage; 
	}
	void SetDamage(int32 Value) 
	{
		Damage = Value; 
	}

protected: //Core methods
	UFUNCTION()
	void SetDamageUILocation();

	UFUNCTION()
	void DestroyDamageUI();

};
