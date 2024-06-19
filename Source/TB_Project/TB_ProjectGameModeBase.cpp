// Copyright Epic Games, Inc. All Rights Reserved.


#include "TB_ProjectGameModeBase.h"
#include "Global.h"

ATB_ProjectGameModeBase::ATB_ProjectGameModeBase()
{
    //DefaultPawnClass = ACPlayer_Revenant::StaticClass();
    CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Characters/Revenant/BP_Revenant.BP_Revenant_C'");
    PlayerControllerClass = AMainPlayerController::StaticClass();
}