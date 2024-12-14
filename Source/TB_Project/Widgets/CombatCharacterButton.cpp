// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CombatCharacterButton.h"
#include "Character/GameCharacter.h"
#include "Global.h"
#include "Components/HealthComponent.h"

void UCombatCharacterButton::AddCharacterName(ACharacter* InCharacter)
{
    FString Name = GetCharacterName(InCharacter->GetName());
    AllCharacters.AddUnique(Name);
}

FString UCombatCharacterButton::GetCharacterName(FString RawName)
{
    FString Tmp;
    RawName = RawName.RightChop(3);
    for (int i = 0; i < RawName.Len(); ++i)
    {
        if (RawName[i] == '_')
        {
            break;
        }

        if (isdigit(RawName[i])) // 숫자가 _보다 먼저 나오면
        {
            FString Str(RawName.Chr(RawName[i]));
            RawName.Split(Str, &RawName, &Tmp);
            return RawName;
        }
    }
    RawName.Split("_", &RawName, &Tmp);
    return RawName;
}

float UCombatCharacterButton::GetHealthProgressBarPercent(ACharacter* InCharacter)
{
    UHealthComponent* HealthComp = CHelpers::GetComponent<UHealthComponent>(InCharacter);
    if (!HealthComp)
    {
        return -1.f;
    }
    float CurHealth = HealthComp->GetHealth();
    float MaxHealth = HealthComp->GetMaxHealth();

    float Value = CurHealth / MaxHealth;

    return 1.f - Value;
}
