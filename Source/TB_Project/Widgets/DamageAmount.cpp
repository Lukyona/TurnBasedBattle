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
}

void ADamageAmount::BeginPlay()
{
	Super::BeginPlay();
	DamageText = Cast<UDamageAmountText>(Widget->GetWidget());

	StartLoc = GetActorLocation();
	EndLoc = StartLoc + FVector(-50.f, -50.f, 100.f);

	FOnTimelineEvent TimelineFinishedCallback;
	TimelineUpdateFunc.BindUFunction(this, "SetDmgAmtLocation");
	TimelineFinishedCallback.BindUFunction(this, "DestroyDmgAmt");
	Timeline->SetTimelineFinishedFunc(TimelineFinishedCallback);

	Timeline->AddInterpFloat(Curve, TimelineUpdateFunc);

	Timeline->PlayFromStart();
}

void ADamageAmount::SetDmgAmtLocation()
{
	FVector loc = UKismetMathLibrary::VLerp(StartLoc, EndLoc, Curve->GetFloatValue(Timeline->GetPlaybackPosition()));
	SetActorLocation(loc);
}

void ADamageAmount::DestroyDmgAmt()
{
	Destroy();
}

void ADamageAmount::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DamageText->SetDamage(Damage);

	FVector CameraLoc = MAINPC->GetCurPlayer()->GetCameraWorldLocation();
	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CameraLoc);
	FRotator NewRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 0.f);
	SetActorRotation(NewRot);
}

