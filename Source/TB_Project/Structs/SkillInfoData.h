#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillInfoData.generated.h"

USTRUCT(BlueprintType)
struct FSkillInfoData : public FTableRowBase
{
    //GENERATED_BODY()
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString SkillKoreanName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString SkillExplanation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTexture* SkillImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int MinDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int MaxDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bIsAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bNeedTarget;
};