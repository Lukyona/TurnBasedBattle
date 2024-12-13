// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	Health = MaxHealth;
}

void UHealthComponent::GetDamage(float Damage)
{
	if ((Health - (int32)Damage) > 0)
	{
		Health -= (int32)Damage;
	}
	else
	{
		Health = 0;
	}
}
