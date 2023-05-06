// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSGameplayTests.h"
#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"
#include "TPS/Items/TPSInventoryItem.h"
#include "TPS/Tests/TestUtils.h"
#include "TPS/Tests/Utils/InputRecordingUtils.h"
#include "TPS/Tests/Utils/JsonUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "TPSGame.Gameplay.InventoryItemCanBeTakenOnJump",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfTooHigh, "TPSGame.Gameplay.InventoryItemCantBeTakenOnJumpIfTooHigh",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsAreTakenOnMovement, "TPSGame.Gameplay.AllItemsAreTakenOnMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllItemsAreTakenOnRecordedMovement, "TPSGame.Gameplay.AllItemsAreTakenOnRecordedMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FMapsShouldBeLoaded, "TPSGame.Gameplay.MapsShouldBeLoaded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, UInputComponent*, InputComponent);
bool FJumpLatentCommand::Update()
{
    JumpPressed(InputComponent);
    return true;
}

bool FInventoryItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/Inventory_TestLevel1");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exists", InventoryItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
        },
        2.0f));

    return true;
}

bool FInventoryItemCantBeTakenOnJumpIfTooHigh::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/Inventory_TestLevel2");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exists", InventoryItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 1);
        },
        2.0f));

    return true;
}

bool FAllItemsAreTakenOnMovement::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/Inventory_TestLevel3");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 7);

    const int32 MoveForwardIndex = GetAxisBindingIndexByName(Character->InputComponent, "Move Forward / Backward");
    TestTrueExpr(MoveForwardIndex != INDEX_NONE);

    const int32 MoveRightIndex = GetAxisBindingIndexByName(Character->InputComponent, "Move Right / Left");
    TestTrueExpr(MoveRightIndex != INDEX_NONE);

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSDuringTimeLatentCommand(
        [=]() { Character->InputComponent->AxisBindings[MoveForwardIndex].AxisDelegate.Execute(1.0f); }, []() {}, 3.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSDuringTimeLatentCommand(
        [=]() { Character->InputComponent->AxisBindings[MoveRightIndex].AxisDelegate.Execute(1.0f); }, []() {}, 2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
            return true;
        }));

    return true;
}

class FSimulateMovementLatentCommand : public IAutomationLatentCommand
{
public:
    FSimulateMovementLatentCommand(UWorld* InWorld, UInputComponent* InInputComponent, const TArray<FBindingsData>& InBindingsData)
        : World(InWorld)
        , InputComponent(InInputComponent)
        , BindingsData(InBindingsData)
    {
    }
    virtual bool Update() override
    {
        if (!World || !InputComponent) return true;

        if (WorldStartTime == 0.0f)
        {
            WorldStartTime = World->TimeSeconds;
        }

        while (World->TimeSeconds - WorldStartTime >= BindingsData[Index].WorldTime)
        {
            for (int32 i = 0; i < InputComponent->AxisBindings.Num(); ++i)
            {
                const float AxisValue = BindingsData[Index].AxisValues[i].Value;
                InputComponent->AxisBindings[i].AxisDelegate.Execute(AxisValue);
            }

            if (Index > 0)
            {
                for (int32 i = 0; i < InputComponent->GetNumActionBindings(); ++i)
                {
                    const auto ActionValue = BindingsData[Index].ActionValues[i];
                    const auto PrevActionValue = BindingsData[Index - 1].ActionValues[i];
                    if (ActionValue.State && ActionValue.State != PrevActionValue.State)
                    {
                        InputComponent->GetActionBinding(i).ActionDelegate.Execute(ActionValue.Key);
                    }
                }
            }

            if (++Index >= BindingsData.Num()) return true;
        }

        return false;
    }

private:
    const UWorld* World;
    const UInputComponent* InputComponent;
    const TArray<FBindingsData> BindingsData;
    int32 Index{0};
    float WorldStartTime{0.0f};
};

void FAllItemsAreTakenOnRecordedMovement::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString MapPath;
        FString JsonName;
        int32 ItemsCount;
    };

    const TArray<FTestData> TestData =  //
        {
            {"MainMap", "/Game/ThirdPerson/Maps/ThirdPersonMap", "CharacterTestInput_MainMap.json", 6},  //
            {"CustomMap", "/Game/ThirdPerson/Maps/CustomMap", "CharacterTestInput_CustomMap.json", 3}    //
        };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.TestName);
        OutTestCommands.Add(FString::Printf(TEXT("%s,%s,%i"), *OneTestData.MapPath, *OneTestData.JsonName, OneTestData.ItemsCount));
    }
}

bool FAllItemsAreTakenOnRecordedMovement::RunTest(const FString& Parameters)
{
    TArray<FString> ParsedParams;
    Parameters.ParseIntoArray(ParsedParams, TEXT(","));
    if (!TestTrue("Map name and JSON param and Items count should exist", ParsedParams.Num() == 3)) return false;

    const auto Level = LevelScope(ParsedParams[0]);

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == FCString::Atoi(*ParsedParams[2]));

    const FString FileName = GetTestDataDir().Append(ParsedParams[1]);
    FInputData InputData;
    if (!JsonUtils::ReadInputData(FileName, InputData)) return false;
    if (!TestTrue("Input data is not empty", InputData.Bindings.Num() > 0)) return false;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    if (!TestNotNull("PlayerController exists", PlayerController)) return false;

    Character->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(World, Character->InputComponent, InputData.Bindings));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
            return true;
        }));

    return true;
}

void FMapsShouldBeLoaded::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    const TArray<TTuple<FString, FString>> TestData =  //
        {
            {"MainMap", "/Game/ThirdPerson/Maps/ThirdPersonMap"},  //
            {"CustomMap", "/Game/ThirdPerson/Maps/CustomMap"}      //
        };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.Key);
        OutTestCommands.Add(OneTestData.Value);
    }
}

bool FMapsShouldBeLoaded::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(Parameters);
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));

    return true;
}

#endif
