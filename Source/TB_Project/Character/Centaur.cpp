// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Centaur.h"
#include "Global.h"

ACentaur::ACentaur()
{
    InitializeFromDataTable("Centaur");

    GetMesh()->SetRelativeLocation(FVector(-51.f, 0, -99.f));
    GetMesh()->SetRelativeScale3D(FVector(0.7f));

    GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);
    GetCapsuleComponent()->SetCapsuleRadius(30.f);
}

void ACentaur::BeginPlay()
{
    Super::BeginPlay();
}
