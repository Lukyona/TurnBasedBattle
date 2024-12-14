// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetingCircle.generated.h"

class USceneComponent;
class UDecalComponent;
class UBoxComponent;

UCLASS()
class TB_PROJECT_API ATargetingCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	ATargetingCircle();
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* Root;

    UPROPERTY(EditDefaultsOnly)
	UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;

	UPROPERTY()
	UMaterialInstance* GreenMaterial;
	UPROPERTY()
	UMaterialInstance* WhiteMaterial;


	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void SetColorGreen();
	void SetColorWhite();

};
