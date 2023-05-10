// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/TPSTurret.h"

#include "TPSProjectile.h"

ATPSTurret::ATPSTurret()
{
    PrimaryActorTick.bCanEverTick = false;

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
    check(TurretMesh);
    SetRootComponent(TurretMesh);
}

void ATPSTurret::BeginPlay()
{
    Super::BeginPlay();

    check(AmmoCount > 0);
    check(FireFrequency > 0.0f);

    const float FirstDelay = FireFrequency;
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ThisClass::OnFire, FireFrequency, true, FirstDelay);
}

void ATPSTurret::OnFire()
{
    if (--AmmoCount == 0)
    {
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
    }

    if (GetWorld())
    {
        const FTransform SocketTransform = TurretMesh->GetSocketTransform("Muzzle");
        if (auto* ProjectileObj = GetWorld()->SpawnActorDeferred<ATPSProjectile>(ProjectileClass, SocketTransform))
        {
            ProjectileObj->SetShotDirection(SocketTransform.GetRotation().GetForwardVector());
            ProjectileObj->FinishSpawning(SocketTransform);
        }
    }
}
