// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetingCircle.generated.h"

UCLASS()
class TB_PROJECT_API ATargetingCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	ATargetingCircle();

	void SetColorGreen();
	void SetColorWhite();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USceneComponent* Root;

    UPROPERTY(EditDefaultsOnly)
	class UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;


	virtual void BeginPlay() override;

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UMaterialInstance* M_Green;
	UMaterialInstance* M_White;

};
