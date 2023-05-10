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
END_DEFINE_SPEC(FTurret)

using namespace TPS::Test;

namespace
{
    const char* MapName = "/Game/Tests/EmptyTestLevel";
    const char* TurretBPName = "Blueprint'/Game/Weapon/BP_TPSTurret.BP_TPSTurret'";
    const FTransform InitialTransform{FVector{1000.0f}};

    void SpecCloseLevel(const UWorld* World)
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            PC->ConsoleCommand(TEXT("Exit"), true);
        }
    }
}

void FTurret::Define()
{
    Describe("Creational",
        [this]()
        {
            BeforeEach([this]()
            {
                AutomationOpenMap(MapName);
                World = GetTestGameWorld();
                TestNotNull("World exists", World);
            });

            It("Cpp instance can't created",
                [this]()
                {
                    const FString ExpectedWarnMsg =
                        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"),
                            *ATPSTurret::StaticClass()->GetName());
                    AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Exact);

                    const ATPSTurret* Turret = World->SpawnActor<ATPSTurret>(ATPSTurret::StaticClass(), InitialTransform);
                    TestNull("Turret doesn't exist", Turret);
                });

            It("Blueprint instance can created",
                [this]()
                {
                    const ATPSTurret* Turret = CreateBlueprint<ATPSTurret>(World, TurretBPName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });

            AfterEach([this]()
            {
                SpecCloseLevel(World);
            });
        });
}

#endif
