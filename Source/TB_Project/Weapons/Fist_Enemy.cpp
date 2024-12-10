// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Fist_Enemy.h"
#include "Global.h"

AFist_Enemy::AFist_Enemy()
{
    Capsule->SetCapsuleRadius(12.f);
    Capsule->SetCapsuleHalfHeight(12.f);

}