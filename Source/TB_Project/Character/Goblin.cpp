// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Goblin.h"
#include "Global.h"

AGoblin::AGoblin()
{
    InitializeFromDataTable("Goblin");

    GetMesh()->SetRelativeLocation(FVector(0, 0, -79.f));
    GetMesh()->SetRelativeScale3D(FVector(1.2f));

    GetCapsuleComponent()->SetCapsuleHalfHeight(80.f);
}

void AGoblin::BeginPlay()
{
    Super::BeginPlay();
}
