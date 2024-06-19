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
		FVector LeftDistance;   // X축 : 높이
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FVector RightDistance;  // X축 : 높이
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FVector PelvisDistance;  // Z축 : 높이
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FRotator LeftRotation;
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FRotator RightRotation;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TB_PROJECT_API UFeetComponent : public UActorComponent
{
	GENERATED_BODY()

	FFeetData Data;

	class ACharacter* OwnerCharacter;
public:	
	UFeetComponent();
	
	FORCEINLINE FFeetData GetData() { return Data; }

	void SetLeftSocketName(FName value) { LeftSocketName = value; }
	void SetRightSocketName(FName value) { RightSocketName = value; }

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		float InterpSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceDistance = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float OffsetDistance = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
		FName LeftSocketName = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "Trace")
		FName RightSocketName = "Foot_R";

	void Trace(FName InName, float& OutDistance, FRotator& OutRotation);


};
