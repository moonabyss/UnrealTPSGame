// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/TPSProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ATPSProjectile::ATPSProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    check(CollisionComponent);
    CollisionComponent->InitSphereRadius(3.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    check(MovementComponent);
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;
}

void ATPSProjectile::BeginPlay()
{
    Super::BeginPlay();

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    CollisionComponent->OnComponentHit.AddDynamic(this, &ATPSProjectile::OnProjectileHit);
    SetLifeSpan(LifeSeconds);
}

void ATPSProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (GetWorld() && OtherActor)
    {
        MovementComponent->StopMovementImmediately();
        OtherActor->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, this);
        Destroy();
    }
}
