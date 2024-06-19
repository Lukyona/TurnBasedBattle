#pragma once

#include "CoreMinimal.h"
#include "CombatCharacterInfo.generated.h"

USTRUCT()
struct FCombatCharacterInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
        FString Name;

    UPROPERTY(EditAnywhere)
        int InitiativeNum = 0;

    UPROPERTY(EditAnywhere)
        class AGameCharacter* Character;
};