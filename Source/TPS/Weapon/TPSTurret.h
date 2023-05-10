// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSTurret.generated.h"

class ATPSProjectile;

UCLASS(Abstract)
class TPS_API ATPSTurret : public AActor
{
    GENERATED_BODY()

public:
    ATPSTurret();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 AmmoCount{10};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Units = s))
    float FireFrequency{1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<ATPSProjectile> ProjectileClass;

private:
    FTimerHandle FireTimerHandle;

    void OnFire();
};
