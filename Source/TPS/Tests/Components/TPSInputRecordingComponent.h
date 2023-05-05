// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPS/Tests/Utils/InputRecordingUtils.h"
#include "TPSInputRecordingComponent.generated.h"

class UPlayerInput;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPS_API UTPSInputRecordingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSInputRecordingComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditAnywhere)
    FString FileName{"CharacterTestInput"};

private:
    UPROPERTY()
    UPlayerInput* PlayerInput;

    FInputData InputData;

    FBindingsData MakeBindingsData() const;
    FString GenerateFileName() const;
};
