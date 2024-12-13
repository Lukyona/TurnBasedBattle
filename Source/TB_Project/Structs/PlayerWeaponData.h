#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerWeaponData.generated.h"

class UAnimMontage;
class UDataTable;

USTRUCT()
struct FPlayerWeaponData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere)
    FString WeaponName;

    UPROPERTY(EditAnywhere)
    UAnimMontage* EquipMontage;

    UPROPERTY(EditAnywhere)
    UAnimMontage* UnEquipMontage;

    UPROPERTY(EditAnywhere)
        TArray<UAnimMontage*> HitMontages;

    UPROPERTY(EditAnywhere)
    UDataTable* SkillInfoDT;

    UPROPERTY(EditAnywhere)
    UDataTable* SkillAnimDT;
};
