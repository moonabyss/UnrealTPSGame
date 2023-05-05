// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "TPSGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseDelegate, bool, IsPaused);

UCLASS(minimalapi)
class ATPSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATPSGameMode();

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause() override;

    UPROPERTY(BlueprintAssignable)
    FOnGamePauseDelegate OnGamePause;
};
