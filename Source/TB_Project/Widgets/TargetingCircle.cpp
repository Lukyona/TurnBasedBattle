// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/TargetingCircle.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstance.h"

#include "Global.h"
#include "Character/CPlayer.h"
#include "Components/StateComponent.h"

ATargetingCircle::ATargetingCircle()
{
	CHelpers::CreateComponent(this, &Root, "Root");
	CHelpers::CreateComponent<UDecalComponent>(this, &Decal, "Decal", Root);
	Decal->SetRelativeRotation(FRotator(90, 0, 0));
	Decal->DecalSize = FVector(40, 90, 90);

	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Decal);
	Box->SetRelativeScale3D(FVector(0.6f, 1, 1));
	
	UMaterialInstance* M_Inst;
	CHelpers::GetAsset<UMaterialInstance>(&M_Inst, "MaterialInstanceConstant'/Game/UI/Circle/TargetingCircle_Material_Inst.TargetingCircle_Material_Inst'");
	Decal->SetDecalMaterial(M_Inst);
	
	CHelpers::GetAsset<UMaterialInstance>(&GreenMaterial, "MaterialInstanceConstant'/Game/UI/Circle/TargetingCircle_Green.TargetingCircle_Green'");
	CHelpers::GetAsset<UMaterialInstance>(&WhiteMaterial, "MaterialInstanceConstant'/Game/UI/Circle/TargetingCircle_White.TargetingCircle_White'");
}

void ATargetingCircle::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ATargetingCircle::OnComponentBeginOverlap);
}

void ATargetingCircle::SetColorGreen()
{
	if (GreenMaterial)
	{
		Decal->SetDecalMaterial(GreenMaterial);
	}
}

void ATargetingCircle::SetColorWhite()
{
	if (WhiteMaterial)
	{
		Decal->SetDecalMaterial(WhiteMaterial);
	}
}

void ATargetingCircle::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
	if (!Player || !MAINPC)
	{
		return;
	}

	UStateComponent* StateComp = CHelpers::GetComponent<UStateComponent>(Player);
	if (StateComp && StateComp->IsMovingMode())
	{
		StateComp->SetWaitMode();
		MAINPC->SetFixedCamera(false);
		MAINPC->ClearMoveAbilityTimer();
		Destroy();
	}
}
