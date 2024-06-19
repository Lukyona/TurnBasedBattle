// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CombatCharacterButton.h"
#include "Character/GameCharacter.h"
#include "Global.h"
#include "Components/HealthComponent.h"

void UCombatCharacterButton::AddCharacterName(ACharacter* Char)
{
    FString name = GetCharacterName(Char->GetName());
    AllCharacters.AddUnique(name);
}

FString UCombatCharacterButton::GetCharacterName(FString name)
{
    FString tmp;
    name = name.RightChop(3);
    for (int i = 0; i < name.Len(); ++i)
    {
        if (name[i] == '_')
            break;

        if (isdigit(name[i])) // 숫자가 _보다 먼저 나오면
        {
            FString str(name.Chr(name[i]));
            name.Split(str, &name, &tmp);
            return name;
        }
    }
    name.Split("_", &name, &tmp);
    return name;
}

float UCombatCharacterButton::GetHealthProgressBarPercent(ACharacter* Char)
{
    UHealthComponent* healthComp = CHelpers::GetComponent<UHealthComponent>(Char);
    float cur = healthComp->GetHealth();
    float max = healthComp->GetMaxHealth();

    float value = cur / max;

    return 1.f - value;
}