// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PauseMenu/SettingOptionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Settings/TPSGameSetting.h"

void USettingOptionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingDisplayName);
    check(SettingCurrentValue);
    check(NextSettingButton);
    check(PrevSettingButton);

    NextSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnNextSetting);
    PrevSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnPrevSetting);
}

void USettingOptionWidget::Init(UTPSGameSetting* InSetting)
{
    Setting = InSetting;
    check(Setting.IsValid());

    UpdateTexts();
}

void USettingOptionWidget::UpdateTexts()
{
    if (Setting.IsValid())
    {
        SettingDisplayName->SetText(FText::FromString(Setting->GetName()));
        SettingCurrentValue->SetText(FText::FromString(Setting->GetCurrentOption().Name));
    }
}

void USettingOptionWidget::OnNextSetting()
{
    if (Setting.IsValid())
    {
        Setting->ApplyNextOption();
        SettingCurrentValue->SetText(FText::FromString(Setting->GetCurrentOption().Name));
    }
}

void USettingOptionWidget::OnPrevSetting()
{
    if (Setting.IsValid())
    {
        Setting->ApplyPrevOption();
        SettingCurrentValue->SetText(FText::FromString(Setting->GetCurrentOption().Name));
    }
}
