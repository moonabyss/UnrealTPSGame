// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSCharacterTests.h"
#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/AutomationTest.h"
#include "TPS/TPSCharacter.h"
#include "TPS/TPSTypes.h"
#include "TPS/Tests/TestUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSCharacterTests, All, All);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthMightBeChangeWithDamage, "TPSGame.Character.HealthMightBeChangeWithDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeKilled, "TPSGame.Character.CharacterCanBeKilled",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoHealShouldRestoreHealth, "TPSGame.Character.AutoHealShouldRestoreHealth",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

using namespace TPS::Test;

namespace
{
    const char* CharacterTestBPName = "/Script/Engine.Blueprint'/Game/Tests/BP_Test_ThirdPersonCharacter.BP_Test_ThirdPersonCharacter'";
}  // namespace

bool FHealthMightBeChangeWithDamage::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    CallFuncByNameWithParam(Character, "SetHealthData", {HealthData.ToString()});

    Character->FinishSpawning(InitialTransform);

    const float DamageAmount = 10.0f;
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health was decreased", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCharacterDestroyedLatentCommand, ATPSCharacter*, Character, float, LifeSpan);
bool FCharacterDestroyedLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime > LifeSpan)
    {
        if (IsValid(Character))
        {
            UE_LOG(LogTPSCharacterTests, Error, TEXT("Character wasn't destroed"));
        }
        return true;
    }

    return false;
}

bool FCharacterCanBeKilled::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    HealthData.LifeSpan = 1.5f;
    CallFuncByNameWithParam(Character, "SetHealthData", {HealthData.ToString()});

    Character->FinishSpawning(InitialTransform);

    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);

    const float KillDamageAmount = HealthData.MaxHealth;
    Character->TakeDamage(KillDamageAmount, FDamageEvent{}, nullptr, nullptr);

    TestEqual("Health is empty", Character->GetHealthPercent(), 0.0f);
    TestTrueExpr(Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None);

    ENUM_LOOP_START(ECollisionChannel, EnumElem)
    if (EnumElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(Character->GetCapsuleComponent()->GetCollisionResponseToChannel(EnumElem) == ECollisionResponse::ECR_Ignore);
    }
    ENUM_LOOP_END

    TestTrueExpr(Character->GetMesh()->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics);
    TestTrueExpr(Character->GetMesh()->IsSimulatingPhysics());
    TestTrueExpr(FMath::IsNearlyEqual(Character->GetLifeSpan(), HealthData.LifeSpan));

    // ADD_LATENT_AUTOMATION_COMMAND(FCharacterDestroyedLatentCommand(Character, HealthData.LifeSpan * 1.1));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [Character]()                                                                    //
        {                                                                                //
            if (IsValid(Character))                                                      //
            {                                                                            //
                UE_LOG(LogTPSCharacterTests, Error, TEXT("Character wasn't destroed"));  //
            }                                                                            //
        },                                                                               //
        HealthData.LifeSpan * 1.1                                                        //
        ));

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FAutoHealCheckLatentCommand, ATPSCharacter*, Character, float, HealingDuration);
bool FAutoHealCheckLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime > HealingDuration)
    {
        if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
        {
            UE_LOG(LogTPSCharacterTests, Error, TEXT("Health is not full"));
        }
        return true;
    }

    return false;
}

bool FAutoHealShouldRestoreHealth::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 200.0f;
    HealthData.HealModifier = 5.0f;
    HealthData.HealRate = 0.5f;
    CallFuncByNameWithParam(Character, "SetHealthData", {HealthData.ToString()});

    Character->FinishSpawning(InitialTransform);

    const float DamageAmount = 10.0f;
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health was decreased", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    const float HealthDiff = HealthData.MaxHealth * (1.0f - Character->GetHealthPercent());
    const float HealingDuration = HealthData.HealRate * (FMath::CeilToInt(HealthDiff / HealthData.HealModifier) + 1);
    // ADD_LATENT_AUTOMATION_COMMAND(FAutoHealCheckLatentCommand(Character, HealingDuration));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [Character]()                                                             //
        {                                                                         //
            if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))       //
            {                                                                     //
                UE_LOG(LogTPSCharacterTests, Error, TEXT("Health is not full"));  //
            }                                                                     //
        },                                                                        //
        HealingDuration                                                           //
        ));

    return true;
}

#endif
