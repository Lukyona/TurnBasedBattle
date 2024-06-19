// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "DamageAmount.generated.h"

UCLASS()
class TB_PROJECT_API ADamageAmount : public AActor
{
	GENERATED_BODY()

	class UDamageAmountText* DamageText;

	FVector StartLoc;
	FVector EndLoc;

    UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* Widget;

	class UTimelineComponent* Timeline;
	//FTimeline Timeline;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		int Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline")
		UCurveFloat* Curve;

public:	
	ADamageAmount();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void SetDmgAmtLocation();

	UFUNCTION()
		void DestroyDmgAmt();
public:	
	virtual void Tick(float DeltaTime) override;

	void SetDamage(int Value) { Damage = Value; }
	int GetDamage() { return Damage; }

	FOnTimelineFloat TimelineUpdateFunc{};

};
