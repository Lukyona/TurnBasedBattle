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

	Box = CreateDefaultSubobject<UBoxComponent>("Box");

	Box->SetupAttachment(Decal);
	Box->SetRelativeScale3D(FVector(0.6f, 1, 1));
	
	UMaterialInstance* m_Inst;
	CHelpers::GetAsset<UMaterialInstance>(&m_Inst, "MaterialInstanceConstant'/Game/UI/Circle/TargetingCircle_Material_Inst.TargetingCircle_Material_Inst'");
	Decal->SetDecalMaterial(m_Inst);
	
	CHelpers::GetAsset<UMaterialInstance>(&M_Green, "MaterialInstanceConstant'/Game/UI/Circle/TargetingCircle_Material_Inst2.TargetingCircle_Material_Inst2'");
	CHelpers::GetAsset<UMaterialInstance>(&M_White, "MaterialInstanceConstant'/Game/UI/Circle/TargetingCircle_Material_Inst3.TargetingCircle_Material_Inst3'");

}

void ATargetingCircle::SetColorGreen()
{
	if (M_Green) Decal->SetDecalMaterial(M_Green);
}

void ATargetingCircle::SetColorWhite()
{
	if (M_White) Decal->SetDecalMaterial(M_White);
}

void ATargetingCircle::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ATargetingCircle::OnComponentBeginOverlap);
}

void ATargetingCircle::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* player = Cast<ACPlayer>(OtherActor);

	if (!player) return;

	UStateComponent* stateComp = CHelpers::GetComponent<UStateComponent>(player);
	if (stateComp->IsMovingMode())
	{
		stateComp->SetWaitMode();
		MAINPC->SetFixedCamera(false);
		MAINPC->ClearMoveAbilityTimer();

		Destroy();
	}

}
