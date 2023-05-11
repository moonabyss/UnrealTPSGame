// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/TPSScreenshots.h"
#include "Camera/CameraActor.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenderingShouldBeCorrect, "TPSGame.Screenshots.RenderingShouldBeCorrect",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority | EAutomationTestFlags::NonNullRHI);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMainPlayerHUDShouldBeRendered, "TPSGame.Screenshots.MainPlayerHUDShouldBeRendered",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority | EAutomationTestFlags::NonNullRHI);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthWidgetShouldBeRenderedCorrectAfterDamage,
    "TPSGame.Screenshots.HealthWidgetShouldBeRenderedCorrectAfterDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority | EAutomationTestFlags::NonNullRHI);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsWidgetBeRenderedWhenGamePaused, "TPSGame.Screenshots.SettingsWidgetBeRenderedWhenGamePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority | EAutomationTestFlags::NonNullRHI);

using namespace TPS::Test;

bool FRenderingShouldBeCorrect::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FVector Location{820.0f, 2080.0f, 150.0f};
    const FRotator Rotation{-20.0f, 100.0f, 0.0f};
    ACameraActor* Camera = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Location, Rotation);
    if (!TestNotNull("Camera exists", Camera)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller exists", PC)) return false;

    PC->SetViewTarget(Camera);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeGameScreenshotLatentCommand("rendering_check_screenshot"));

    return true;
}

bool FMainPlayerHUDShouldBeRendered::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");
    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("main_player_hud_screenshot"));

    return true;
}

bool FHealthWidgetShouldBeRenderedCorrectAfterDamage::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller exists", PC)) return false;
    if (!TestNotNull("Pawn exists", PC->GetPawn())) return false;

    const float DamageAmount{40.0f};
    PC->GetPawn()->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("health_widget_screenshot"));

    return true;
}

bool FSettingsWidgetBeRenderedWhenGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPerson/Maps/ThirdPersonMap");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller exists", PC)) return false;

    PausePressed(PC->InputComponent);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("settings_widget_screenshot"));

    return true;
}

#endif
