#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TestUtils.h"
#include "Misc/OutputDeviceNull.h"

namespace TPS
{
    namespace Test
    {

        UWorld* GetTestGameWorld()
        {
            const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
            for (const FWorldContext& Context : WorldContexts)
            {
                if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World())
                {
                    return Context.World();
                }
            }

            return nullptr;
        }

        void CallFuncByNameWithParam(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
        {
            if (!Object) return;

            FString Command = FString::Printf(TEXT("%s"), *FuncName);
            for (const auto Param : Params)
            {
                Command.Append(" ").Append(Param);
            }

            FOutputDeviceNull OutputDeviceNull;
            Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
        }

        int32 GetActionBindingIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent)
        {
            if (!InputComp) return INDEX_NONE;

            for (int32 i = 0; i < InputComp->GetNumActionBindings(); ++i)
            {
                const FInputActionBinding Action = InputComp->GetActionBinding(i);
                if (Action.GetActionName().ToString().Equals(ActionName) && Action.KeyEvent == InputEvent)
                {
                    return i;
                }
            }
            return INDEX_NONE;
        }

        int32 GetAxisBindingIndexByName(UInputComponent* InputComp, const FString& AxisName)
        {
            if (!InputComp) return INDEX_NONE;

            const int32 AxisIndex = InputComp->AxisBindings.IndexOfByPredicate(
                [&](const FInputAxisBinding& AxisBind) { return AxisBind.AxisName.ToString().Equals(AxisName); });

            return AxisIndex;
        }

        FTPSDuringTimeLatentCommand::FTPSDuringTimeLatentCommand(
            TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout)
            : Callback(MoveTemp(InCallback))
            , TimeoutCallback(MoveTemp(InTimeoutCallback))
            , Timeout(InTimeout)
        {
        }

        bool FTPSDuringTimeLatentCommand::Update()
        {
            const double NewTime = FPlatformTime::Seconds();
            if (NewTime - StartTime >= Timeout)
            {
                TimeoutCallback();
                return true;
            }

            Callback();
            return false;
        }

        FString GetTestDataDir()
        {
            return FPaths::GameSourceDir().Append("TPS/Tests/Data/");
        }

        UWidget* FindWidgetByName(const UUserWidget* Widget, const FName& WidgetName)
        {
            if (!Widget || !Widget->WidgetTree) return nullptr;

            UWidget* FoundWidget = nullptr;
            UWidgetTree::ForWidgetAndChildren(Widget->WidgetTree->RootWidget,
                [&](UWidget* Child)
                {
                    if (Child && Child->GetFName().IsEqual(WidgetName))
                    {
                        FoundWidget = Child;
                    }
                });
            return FoundWidget;
        }

        void DoInputAction(UInputComponent* InputComponent, const FString& ActionName, const FKey& Key)
        {
            if (!InputComponent) return;

            const int32 ActionIndex = GetActionBindingIndexByName(InputComponent, ActionName, EInputEvent::IE_Pressed);
            if (ActionIndex != INDEX_NONE)
            {
                const auto ActionBind = InputComponent->GetActionBinding(ActionIndex);
                ActionBind.ActionDelegate.Execute(Key);
            }
        }
        void JumpPressed(UInputComponent* InputComponent)
        {
            DoInputAction(InputComponent, "Jump", EKeys::SpaceBar);
        }
        void PausePressed(UInputComponent* InputComponent)
        {
            DoInputAction(InputComponent, "ToggleGamePause", EKeys::P);
        }

    }  // namespace Test
}  // namespace TPS

#endif
