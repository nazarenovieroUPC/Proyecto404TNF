#include "Actors/ItemMoneda.h"
#include "Components/InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

AItemMoneda::AItemMoneda()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    bReplicates = true;
    SetReplicateMovement(true);

    if (MeshItem)
    {
        MeshItem->SetSimulatePhysics(true);
        MeshItem->SetEnableGravity(true);
        MeshItem->SetLinearDamping(1.8f);
        MeshItem->SetAngularDamping(2.5f);
        MeshItem->SetUseCCD(true); 
        
        MeshItem->SetCollisionProfileName(TEXT("Custom"));
        MeshItem->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        
        MeshItem->SetCollisionResponseToAllChannels(ECR_Ignore);
        MeshItem->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
        MeshItem->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
        
        MeshItem->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        MeshItem->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
        MeshItem->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        MeshItem->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
    }
}

void AItemMoneda::BeginPlay()
{
    Super::BeginPlay();

    if (MeshItem)
    {
        MeshItem->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        MeshItem->SetUseCCD(true);
    }

    if (HasAuthority())
    {
        FTimerHandle TimerRetardo;
        GetWorldTimerManager().SetTimer(TimerRetardo, this, &AItemMoneda::IniciarBusqueda, RetardoAtraccion, false);
    }
}

void AItemMoneda::IniciarBusqueda()
{
    GetWorldTimerManager().SetTimer(TimerDeteccion, this, &AItemMoneda::BuscarJugadorCercano, 0.1f, true);
}

void AItemMoneda::BuscarJugadorCercano()
{
    if (!HasAuthority() || TargetActor) return;

    APawn* MejorPawn = nullptr;
    float MejorDistSq = FMath::Square(MagnetRadius);
    const FVector PosActual = GetActorLocation();
    
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PC = It->Get())
        {
            if (APawn* Pawn = PC->GetPawn())
            {
                if (Pawn->FindComponentByClass<UInventoryComponent>())
                {
                    const float DistSq = FVector::DistSquared(PosActual, Pawn->GetActorLocation());
                    if (DistSq < MejorDistSq)
                    {
                        MejorDistSq = DistSq;
                        MejorPawn = Pawn;
                    }
                }
            }
        }
    }
    
    if (MejorPawn)
    {
        TargetActor = MejorPawn;
        GetWorldTimerManager().ClearTimer(TimerDeteccion);

        if (MeshItem)
        {
            MeshItem->SetSimulatePhysics(false);
            MeshItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
        
        SetActorTickEnabled(true);
    }
}

void AItemMoneda::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!HasAuthority() || !TargetActor) return;
    
    const FVector PosicionDestino = TargetActor->GetActorLocation() + FVector(0.0f, 0.0f, 40.0f);
    const FVector PosicionActual = GetActorLocation();

    const FVector NuevaPosicion = FMath::VInterpTo(PosicionActual, PosicionDestino, DeltaTime, FlySpeed / 100.0f);
    SetActorLocation(NuevaPosicion);

    if (FVector::DistSquared(NuevaPosicion, PosicionDestino) <= FMath::Square(PickupDistance))
    {
        if (UInventoryComponent* Inventario = TargetActor->FindComponentByClass<UInventoryComponent>())
        {
            Inventario->AddMonedas(DataDelItem.Cantidad > 0 ? DataDelItem.Cantidad : 1);
        }
        Destroy();
    }
}