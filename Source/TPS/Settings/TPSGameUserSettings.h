// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"

#include "TPSGameUserSettings.generated.h"

class UTPSGameSetting;

UCLASS()
class TPS_API UTPSGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    UTPSGameUserSettings();
    static UTPSGameUserSettings* Get();

    const TArray<UTPSGameSetting*>& GetVideoSettings() const;

private:
    UPROPERTY()
    TArray<UTPSGameSetting*> VideoSettings;
};
