// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSGameUserSettingsTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Settings/TPSGameUserSettings.h"
#include "Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsShouldExists, "TPSGame.GameUserSettings.SettingsShouldExists",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

using namespace TPS::Test;

bool FSettingsShouldExists::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");

    TestTrueExpr(UTPSGameUserSettings::Get() != nullptr);
    TestTrueExpr(UTPSGameUserSettings::Get()->GetVideoSettings().Num() == 8);

    return true;
}

#endif
