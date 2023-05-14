// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSInventoryComponentTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Components/TPSInventoryComponent.h"
#include "TPS/TPSTypes.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCouldBeCreated, "TPSGame.Components.Inventory.ComponentCouldBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemScoresShouldBeZerosByDefault, "TPSGame.Components.Inventory.ItemScoresShouldBeZerosByDefault",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeScoreShouldntBeAdded, "TPSGame.Components.Inventory.NegativeScoreShouldntBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveScoreShouldBeAdded, "TPSGame.Components.Inventory.PositiveScoreShouldBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScoreMoreThanLimitCantBeAdded, "TPSGame.Components.Inventory.ScoreMoreThanLimitCantBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

using namespace TPS::Test;

namespace
{
    class UTPSInventoryComponentTestable : public UTPSInventoryComponent
    {
    public:
        void SetLimits(const TMap<EInventoryItemType, int32>& Limits) { InventoryLimits = Limits; }
    };

    TMap<EInventoryItemType, int32> InitLimits(UTPSInventoryComponentTestable* InvComp, int32 LimitValue)
    {
        TMap<EInventoryItemType, int32> InvLimits;
        ENUM_LOOP_START(EInventoryItemType, EnumElem)
        InvLimits.Add(EnumElem, LimitValue);
        ENUM_LOOP_END
        InvComp->SetLimits(InvLimits);

        return InvLimits;
    }
}  // namespace

bool FComponentCouldBeCreated::RunTest(const FString& Parameters)
{
    const UTPSInventoryComponent* InvComp = NewObject<UTPSInventoryComponent>();
    if (!TestNotNull("Inventory component exists", InvComp))
    {
        return false;
    }

    return true;
}

bool FItemScoresShouldBeZerosByDefault::RunTest(const FString& Parameters)
{
    const UTPSInventoryComponent* InvComp = NewObject<UTPSInventoryComponent>();
    if (!TestNotNull("Inventory component exists", InvComp))
    {
        return false;
    }

    /*ENUM_LOOP_START(EInventoryItemType, EnumElem)
    TestTrueExpr(InvComp->GetInventoryAmountByType(EnumElem) == 0);
    ENUM_LOOP_END*/

    ForEach<EInventoryItemType>(
        [&](EInventoryItemType EnumElem)
        {
            TestTrueExpr(InvComp->GetInventoryAmountByType(EnumElem) == 0);
        });

    return true;
}

bool FNegativeScoreShouldntBeAdded::RunTest(const FString& Parameters)
{
    UTPSInventoryComponentTestable* InvComp = NewObject<UTPSInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exists", InvComp))
    {
        return false;
    }

    InitLimits(InvComp, 100);

    const int32 NegativeScoreAMount = -1;
    const int32 InitialScoreAmount = 10;

    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    TestTrueExpr(InvComp->TryToAddItem({EnumElem, InitialScoreAmount}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(EnumElem) == InitialScoreAmount);
    TestTrueExpr(!InvComp->TryToAddItem({EnumElem, NegativeScoreAMount}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(EnumElem) == InitialScoreAmount);
    ENUM_LOOP_END

    return true;
}

bool FPositiveScoreShouldBeAdded::RunTest(const FString& Parameters)
{
    UTPSInventoryComponentTestable* InvComp = NewObject<UTPSInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exists", InvComp))
    {
        return false;
    }

    const auto InvLimits = InitLimits(InvComp, 100);

    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    for (int32 i = 0; i < InvLimits[EnumElem]; ++i)
    {
        TestTrueExpr(InvComp->TryToAddItem({EnumElem, 1}));
        TestTrueExpr(InvComp->GetInventoryAmountByType(EnumElem) == i + 1);
    }
    ENUM_LOOP_END

    return true;
}

bool FScoreMoreThanLimitCantBeAdded::RunTest(const FString& Parameters)
{
    UTPSInventoryComponentTestable* InvComp = NewObject<UTPSInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exists", InvComp))
    {
        return false;
    }

    const int32 ScoreLimit = 100;
    InitLimits(InvComp, ScoreLimit);

    TestTrueExpr(InvComp->TryToAddItem({EInventoryItemType::SPHERE, 10}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 10);
    TestTrueExpr(!InvComp->TryToAddItem({EInventoryItemType::SPHERE, ScoreLimit + 1}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 10);

    return true;
}

#endif
