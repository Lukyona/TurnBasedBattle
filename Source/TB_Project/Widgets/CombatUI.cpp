// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CombatUI.h"
#include "Global.h"
#include "Components/HealthComponent.h"
#include "Widgets/CombatCharacterButton.h"

void UCombatUI::UpdateCharButtons(int32 TurnNum)
{
    if (TurnNum == 0)
    {
        CombatCharacterButtons[CombatCharacterButtons.Num() - 1]->ScaleDown();
    }
    else
    {
        CombatCharacterButtons[TurnNum - 1]->ScaleDown();
    }
    
    CombatCharacterButtons[TurnNum]->ScaleUp();
}

UCombatCharacterButton* UCombatUI::GetDeadCharButton(ACharacter* Character)
{
    for (UCombatCharacterButton* CharButton : CombatCharacterButtons)
    {
        if (CharButton->GetCharacter() == Character)
        {
            CharButton->ButtonDown();
            return CharButton;
        }
    }
    return nullptr;
}

void UCombatUI::UpdateEnemyHealth(ACharacter* Enemy)
{
    UHealthComponent* HealthComp = CHelpers::GetComponent<UHealthComponent>(Enemy);
    if (HealthComp)
    {
        int32 CurHealth = HealthComp->GetHealth();
        int32 MaxHealth = HealthComp->GetMaxHealth();
        SetEnemyHealth(CurHealth, MaxHealth);
    }
}
