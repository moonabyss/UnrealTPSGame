// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "VideoSettingsWidget.generated.h"

class UButton;
class USettingOptionWidget;
class UVerticalBox;

UCLASS()
class TPS_API UVideoSettingsWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VideoSettingsContainer;

    UPROPERTY(meta = (BindWidget))
    UButton* RunBenchmarkButton;

    UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<USettingOptionWidget> SettingOptionWidgetClass;

private:
    UFUNCTION()
    void OnBenchmark();

    void OnVideoSettingsUpdated();
};
