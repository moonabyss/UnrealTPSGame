// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayerController.h"

void ATPSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    check(InputComponent);
    InputComponent->BindAction("ToggleGamePause", IE_Pressed, this, &ThisClass::ToggleGamePause).bExecuteWhenPaused = true;
}

void ATPSPlayerController::ToggleGamePause()
{
    bInPause = !bInPause;
    SetPause(bInPause);
    bShowMouseCursor = bInPause;
    bInPause                                                                     //
        ? SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false))  //
        : SetInputMode(FInputModeGameOnly());

    OnGamePause.Broadcast(bInPause);
}
