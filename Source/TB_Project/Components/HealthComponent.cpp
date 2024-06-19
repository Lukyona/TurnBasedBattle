// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	Health = MaxHealth;
}

void UHealthComponent::GetDamage(float Damage)
{
	if ((Health - (int)Damage) > 0)
		Health -= (int)Damage;
	else
		Health = 0;
}
