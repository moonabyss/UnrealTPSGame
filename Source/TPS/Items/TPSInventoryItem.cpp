// My game copyright

#include "TPS/Items/TPSInventoryItem.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "TPS/Components/TPSInventoryComponent.h"

ATPSInventoryItem::ATPSInventoryItem()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    check(CollisionComponent);
    CollisionComponent->InitSphereRadius(30.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CollisionComponent->SetGenerateOverlapEvents(true);
    SetRootComponent(CollisionComponent);
}

void ATPSInventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (const auto Pawn = Cast<APawn>(OtherActor))
    {
        if (const auto InvComp = Pawn->FindComponentByClass<UTPSInventoryComponent>())
        {
            if (InvComp->TryToAddItem(InventoryData))
            {
                Destroy();
            }
        }
    }
}
