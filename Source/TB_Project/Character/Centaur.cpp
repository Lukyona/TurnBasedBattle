// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Centaur.h"
#include "Global.h"

static const FName CentaurKey(TEXT("Centaur"));

ACentaur::ACentaur()
{
    InitializeFromDataTable(CentaurKey);
    SetTransform(FVector(-51.f, 0, -99.f), FVector(0.7f));
    SetCapsuleSize(100.f, 30.f);
}
