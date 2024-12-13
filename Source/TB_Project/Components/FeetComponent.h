// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector LeftEffector;
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FVector RightEffector;
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FRotator LeftRotation;
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	FRotator RightRotation;
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
	float IKHipOffset;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UFeetComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UFeetComponent();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FFeetData Data;

	class ACharacter* OwnerCharacter;

	float InterpSpeed = 50.f;
	float TraceDistance = 200.f;
	float OffsetDistance = 5.f;
	FName LeftSocketName = "Foot_L_Socket";
	FName RightSocketName = "Foot_R_Socket";

public:
	FORCEINLINE FFeetData GetData()
	{
		return Data; 
	}

private:
	void Trace(FName InName, float& OutFootOffset, FRotator& OutRotation, FVector& OutHitLocation);

};
