// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CombatUI.h"
#include "Global.h"
#include "Components/HealthComponent.h"
#include "Widgets/CombatCharacterButton.h"


void UCombatUI::UpdateCharButtons(int Num)
{
    if (Num == 0)
        CombatCharacterButtons[CombatCharacterButtons.Num() - 1]->ScaleDown();
    else
        CombatCharacterButtons[Num - 1]->ScaleDown();
    
    CombatCharacterButtons[Num]->ScaleUp();

}

UCombatCharacterButton* UCombatUI::GetDeadCharButton(ACharacter* Character)
{
    for (UCombatCharacterButton* ccb : CombatCharacterButtons)
    {
        if (ccb->GetCharacter() == Character)
        {
            ccb->ButtonDown();
            return ccb;
        }
    }
    return nullptr;
}

void UCombatUI::UpdateEnemyHealth(ACharacter* Enemy)
{
    UHealthComponent* healthComp = CHelpers::GetComponent<UHealthComponent>(Enemy);
    int cur = healthComp->GetHealth();
    int max = healthComp->GetMaxHealth();
    SetEnemyHealth(cur, max);
}

