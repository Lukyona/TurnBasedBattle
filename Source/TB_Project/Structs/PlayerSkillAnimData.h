#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerSkillAnimData.generated.h"

USTRUCT()
struct FPlayerSkillAnimData : public FTableRowBase
{
    //GENERATED_BODY()
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere)
        UAnimMontage* SkillMontage;

    UPROPERTY(EditAnywhere)
        FString SkillName;
};
