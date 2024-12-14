// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DamageAmount.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"

#include "Global.h"
#include "Character/CPlayer.h"
#include "Widgets/DamageAmountText.h"

ADamageAmount::ADamageAmount()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UWidgetComponent>(this, &Widget, "Widget", GetRootComponent());

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	Timeline->SetTimelineLength(1.75f);

	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/UI/DmgAmt_Curve.DmgAmt_Curve'");
}

void ADamageAmount::BeginPlay()
{
	Super::BeginPlay();

	DamageText = Cast<UDamageAmountText>(Widget->GetWidget());

	StartLoc = GetActorLocation();
	EndLoc = StartLoc + FVector(-50.f, -50.f, 100.f);

	FOnTimelineFloat TimelineUpdateFunc;
	TimelineUpdateFunc.BindUFunction(this, "SetDamageUILocation");

	FOnTimelineEvent TimelineFinishedCallback;
	TimelineFinishedCallback.BindUFunction(this, "DestroyDamageUI");

	Timeline->AddInterpFloat(Curve, TimelineUpdateFunc);
	Timeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
	Timeline->PlayFromStart();
}

void ADamageAmount::SetDamageUILocation()
{
	FVector Location = FMath::Lerp(StartLoc, EndLoc, Curve->GetFloatValue(Timeline->GetPlaybackPosition()));
	SetActorLocation(Location);
}

void ADamageAmount::DestroyDamageUI()
{
	if (this->IsValidLowLevel())
	{
		Destroy();
	}
}

void ADamageAmount::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(DamageText && DamageText->GetDamage() == 0)
	{
		DamageText->SetDamage(Damage);
	}

	FVector CameraLoc = MAINPC->GetCurPlayer()->GetCameraWorldLocation();
	if (FVector::DistSquared(CameraLoc, PreviousCameraLoc) > FMath::Square(5.f)) // 움직임이 일정 거리 이상일 때만 업데이트
	{
		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CameraLoc);
		FRotator NewRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 50.f);
		SetActorRotation(NewRot);
	}
	PreviousCameraLoc = CameraLoc; // 이전 카메라 위치를 업데이트
}
