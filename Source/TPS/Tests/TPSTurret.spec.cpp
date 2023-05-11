// Copyright Epic Games, Inc. All Rights Reserved.

#include "Kismet/GameplayStatics.h"
#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Weapon/TPSTurret.h"

BEGIN_DEFINE_SPEC(FTurret, "TPSGame.Turret",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
ATPSTurret* Turret;
END_DEFINE_SPEC(FTurret)

using namespace TPS::Test;

namespace
{
    const char* MapName = "/Game/Tests/EmptyTestLevel";
    const char* TurretBPName = "Blueprint'/Game/Weapon/BP_TPSTurret.BP_TPSTurret'";
    const char* TurretBPTestName = "Blueprint'/Game/Tests/BP_Test_TPSTurret.BP_Test_TPSTurret'";
    const FTransform InitialTransform{FVector{0.0f, 286.0f, 20.0f}};
}  // namespace

void FTurret::Define()
{
    Describe("Creational",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);
                });

            It("Cpp instance can't created",
                [this]()
                {
                    const FString ExpectedWarnMsg =
                        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSTurret::StaticClass()->GetName());
                    AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Exact);

                    ATPSTurret* Turret = World->SpawnActor<ATPSTurret>(ATPSTurret::StaticClass(), InitialTransform);
                    TestNull("Turret doesn't exist", Turret);
                });

            It("Blueprint instance can created",
                [this]()
                {
                    ATPSTurret* Turret = CreateBlueprint<ATPSTurret>(World, TurretBPName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });

            AfterEach([this]() { SpecCloseLevel(World); });
        });

    Describe("Defaults",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);
                    Turret = CreateBlueprint<ATPSTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });

            const TArray<TTuple<int32, float>> TestData{{45, 2.0f}};
            for (const auto& Data : TestData)
            {
                const auto TestName = FString::Printf(TEXT("Ammo: %i and freq: %.0f should be set up correctly"), Data.Key, Data.Value);
                It(TestName,
                    [this, Data]()
                    {
                        const auto [Ammo, Freq] = Data;
                        CallFuncByNameWithParam(Turret, "SetTurretData", {FString::FromInt(Ammo), FString::SanitizeFloat(Freq)});
                        const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                        TestTrueExpr(AmmoCount == Ammo);

                        const float FireFrequency = GetPropertyValueByName<ATPSTurret, float>(Turret, "FireFrequency");
                        TestTrueExpr(FireFrequency == Freq);
                    });
            }

            AfterEach([this]() { SpecCloseLevel(World); });
        });

    Describe("Ammo",
        [this]()
        {
            const int32 InitialAmmoCount = 3;
            const float FireFreq = 1.0f;
            LatentBeforeEach(
                [this, InitialAmmoCount, FireFreq](const FDoneDelegate& TestDone)
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);
                    Turret = CreateBlueprint<ATPSTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                    CallFuncByNameWithParam(
                        Turret, "SetTurretData", {FString::FromInt(InitialAmmoCount), FString::SanitizeFloat(FireFreq)});
                    TestDone.Execute();
                });

            const auto TestName = FString::Printf(TEXT("Should be empty after %i sec"), FMath::RoundToInt(InitialAmmoCount * FireFreq));
            LatentIt(TestName, EAsyncExecution::ThreadPool,
                [this, InitialAmmoCount, FireFreq](const FDoneDelegate& TestDone)
                {
                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                            TestTrueExpr(AmmoCount == InitialAmmoCount);
                        });

                    const float SyncDelta = 0.5f;
                    FPlatformProcess::Sleep(InitialAmmoCount * FireFreq + SyncDelta);

                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                            TestTrueExpr(AmmoCount == 0);
                        });

                    TestDone.Execute();
                });

            LatentAfterEach(
                [this](const FDoneDelegate& TestDone)
                {
                    SpecCloseLevel(World);
                    TestDone.Execute();
                });
        });
}

#endif