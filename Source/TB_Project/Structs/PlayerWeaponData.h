#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerWeaponData.generated.h"

USTRUCT()
struct FPlayerWeaponData : public FTableRowBase
{
    //GENERATED_BODY()
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere)
        FString WeaponName;

    UPROPERTY(EditAnywhere)
        class UAnimMontage* EquipMontage;

    UPROPERTY(EditAnywhere)
        class UAnimMontage* UnEquipMontage;

    UPROPERTY(EditAnywhere)
        TArray<class UAnimMontage*> HitMontages;

    UPROPERTY(EditAnywhere)
        class UDataTable* SkillInfoDT;

    UPROPERTY(EditAnywhere)
        class UDataTable* SkillAnimDT;
};
