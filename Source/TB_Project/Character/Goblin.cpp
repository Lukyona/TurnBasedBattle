// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Goblin.h"
#include "Global.h"

static const FName GoblinKey(TEXT("Goblin"));

AGoblin::AGoblin()
{
    InitializeFromDataTable(GoblinKey);
    SetTransform(FVector(0, 0, -79.f), FVector(1.2f));
    SetCapsuleSize(80.f);
}
