// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSGameMode.h"

#include "UObject/ConstructorHelpers.h"

#include "TPSCharacter.h"

ATPSGameMode::ATPSGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

bool ATPSGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate /*= FCanUnpause()*/)
{
    const bool Succeed = Super::SetPause(PC, CanUnpauseDelegate);

    if (Succeed)
    {
        OnGamePause.Broadcast(true);
    }

    return Succeed;
}

bool ATPSGameMode::ClearPause()
{
    const bool Succeed = Super::ClearPause();

    if (Succeed)
    {
        OnGamePause.Broadcast(false);
    }

    return Succeed;
}
