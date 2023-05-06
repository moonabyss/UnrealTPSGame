// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSPlayerControllerTests.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Misc/AutomationTest.h"

#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBePaused, "TPSGame.PlayerController.CanBePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBeUnPaused, "TPSGame.PlayerController.CanBeUnPaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

bool FGameCanBePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);
    PausePressed(PC->InputComponent);
    TestTrueExpr(PC->IsPaused());
    TestTrueExpr(PC->bShowMouseCursor);

    return true;
}

bool FGameCanBeUnPaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);
    PausePressed(PC->InputComponent);
    PausePressed(PC->InputComponent);
    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);

    return true;
}

#endif
