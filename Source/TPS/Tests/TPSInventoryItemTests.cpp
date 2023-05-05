// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSInventoryItemTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Items/TPSInventoryItem.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TPS/Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TPS/TPSCharacter.h"
#include "TPS/Components/TPSInventoryComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "TPSGame.Items.Inventory.CppActorCantBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "TPSGame.Items.Inventory.BlueprintShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectly, "TPSGame.Items.Inventory.InventoryDataShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryCanBeTaken, "TPSGame.Items.Inventory.InventoryCanBeTaken",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEveryInventoryItemMeshExists, "TPSGame.Items.Inventory.EveryInventoryItemMeshExists",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
constexpr char* InventoryItemBPName = "/Script/Engine.Blueprint'/Game/Inventory/BP_TPSInventoryItem.BP_TPSInventoryItem'";
constexpr char* InventoryItemBPTestName = "/Script/Engine.Blueprint'/Game/Tests/BP_Test_TPSInventoryItem.BP_Test_TPSInventoryItem'";
}  // namespace

using namespace TPS::Test;

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
    const FString ExpectedWarnMsg =
        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSInventoryItem::StaticClass()->GetName());
    AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Exact);

    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    FTransform InitialTransform{FVector{1000.0f}};
    const ATPSInventoryItem* InvItem = World->SpawnActor<ATPSInventoryItem>(ATPSInventoryItem::StaticClass(), InitialTransform);
    if (!TestNull("Inventory item exists", InvItem))
    {
        return false;
    }

    return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector{1000.0f}};
    const ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem))
    {
        return false;
    }

    const auto CollisionComp = InvItem->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("Sphere component exists", CollisionComp))
    {
        return false;
    }

    /*TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() > 0.0f);
    TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
    TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::ECR_Overlap);
    TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
    TestTrueExpr(CollisionComp->GetAttachmentRoot() == CollisionComp);*/

    ENUM_LOOP_START(ECollisionChannel, EnumElem)
    if (EnumElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EnumElem) == ECollisionResponse::ECR_Overlap);
    }
    ENUM_LOOP_END

    const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text render component exists", TextRenderComp))
    {
        return false;
    }

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static mesh component exists", StaticMeshComp))
    {
        return false;
    }

    TestTrueExpr(StaticMeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

bool FInventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector{1000.0f}};
    ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem))
    {
        return false;
    }

    const FInventoryData InvData{EInventoryItemType::CUBE, 13};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParam(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text render component exists", TextRenderComp))
    {
        return false;
    }

    TestTrueExpr(TextRenderComp->Text.ToString().Equals(FString::FromInt(InvData.Score)));
    TestTrueExpr(TextRenderComp->TextRenderColor == Color.ToFColor(true));

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static mesh component exists", StaticMeshComp))
    {
        return false;
    }

    const auto Material = StaticMeshComp->GetMaterial(0);
    if (!TestNotNull("Material exist", Material)) return false;

    FLinearColor MaterialColor;
    Material->GetVectorParameterValue(FHashedMaterialParameterInfo{"Color"}, MaterialColor);
    TestTrueExpr(MaterialColor == Color);

    return true;
}

bool FInventoryCanBeTaken::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector{1000.0f}};
    ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem))
    {
        return false;
    }

    const FInventoryData InvData{EInventoryItemType::CUBE, 13};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParam(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSCharacter::StaticClass(), Pawns);
    if (!TestTrueExpr(Pawns.Num() == 1))
    {
        return false;
    };

    const auto Character = Cast<ATPSCharacter>(Pawns[0]);
    if (!TestNotNull("TPSCharacter exists", Character))
    {
        return false;
    }

    const auto InvComp = Character->FindComponentByClass<UTPSInventoryComponent>();
    if (!TestNotNull("InvComp exists", InvComp))
    {
        return false;
    }
    TestTrueExpr(InvComp->GetInventoryAmountByType(InvData.Type) == 0);

    // Character takes pickup
    Character->SetActorLocation(InitialTransform.GetLocation());

    TestTrueExpr(InvComp->GetInventoryAmountByType(InvData.Type) == InvData.Score);
    TestTrueExpr(!IsValid(InvItem));

    TArray<AActor*> InvItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InvItems);
    TestTrueExpr(InvItems.Num() == 0);

    return true;
}

bool FEveryInventoryItemMeshExists::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ENUM_LOOP_START(EInventoryItemType, EnumElem)

    const FTransform InitialTransform{FVector{100.0f * (Index + 1)}};
    ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem)) return false;

    const FInventoryData InvData{EnumElem, 13};
    const FLinearColor Color = FLinearColor::Red;
    CallFuncByNameWithParam(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static mesh component exists", StaticMeshComp)) return false;

    const FString MeshMsg = FString::Printf(TEXT("Static mesh for %s exists"), *UEnum::GetValueAsString(EnumElem));
    TestNotNull(*MeshMsg, StaticMeshComp->GetStaticMesh().Get());

    ENUM_LOOP_END

    return true;
}

#endif
