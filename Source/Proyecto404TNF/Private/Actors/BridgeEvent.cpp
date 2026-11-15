// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/BridgeEvent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/HordeManagerComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameStates/MisionSystemState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Proyecto404TNF/Proyecto404TNFCharacter.h"

ABridgeEvent::ABridgeEvent()
{
    PrimaryActorTick.bCanEverTick = false;
    
    bReplicates = true;
    bBridgeBuilt = false;
    
    StartPoint = CreateDefaultSubobject<USceneComponent>("RootComp");
    SetRootComponent(StartPoint);
    
    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    BoxCollision->SetupAttachment(RootComponent);
    BoxCollision->SetLineThickness(5);
    BoxCollision->SetBoxExtent(FVector(100, 100, 100));
    BoxCollision->SetHiddenInGame(false);
    BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    MeshBridge = CreateDefaultSubobject<UStaticMeshComponent>("MeshBridge");
    MeshBridge->SetupAttachment(RootComponent);
    MeshBridge->SetVisibility(false);
    MeshBridge->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    MeshBridgeInstance = CreateDefaultSubobject<UInstancedStaticMeshComponent>("MeshBridgeInstance");
    MeshBridgeInstance->SetupAttachment(RootComponent);
    MeshBridgeInstance->bNavigationRelevant = true;

    // Flecha verde que marca dónde revivirán los jugadores tras cruzar/armar el puente
    PuntoRespawnPuente = CreateDefaultSubobject<UArrowComponent>(TEXT("PuntoRespawnPostPuente"));
    PuntoRespawnPuente->SetupAttachment(RootComponent);
    PuntoRespawnPuente->SetRelativeLocation(FVector(1100.0f, 0.0f, 100.0f));
    PuntoRespawnPuente->ArrowSize = 2.0f;
    PuntoRespawnPuente->ArrowColor = FColor::Green;
    
    HordeManagerComponent = CreateDefaultSubobject<UHordeManagerComponent>("HordeManagerComponent");
}

void ABridgeEvent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ABridgeEvent, bBridgeBuilt);
    DOREPLIFETIME(ABridgeEvent, SegmentsBuilt);
}

void ABridgeEvent::OnRep_BridgeBuilt()
{
    if (bBridgeBuilt && MeshBridge)
    {
       MeshBridge->SetVisibility(true);
       MeshBridge->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
       
       BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
       
       if (GEngine)
       {
          GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("¡Ya puedes activar el puente para cruzar!"));
       }
    }
}

void ABridgeEvent::CanConstruct()
{
    if (HasAuthority())
    {
       bBridgeBuilt = true;
       OnRep_BridgeBuilt();
    }
}

void ABridgeEvent::BeginPlay()
{
    Super::BeginPlay();
    
    if (HordeManagerComponent)
    {
        HordeManagerComponent->OnHordeVictory.AddDynamic(this, &ABridgeEvent::OnHordeCompleted);
    }
    
    if (HasAuthority())
    {
       AMisionSystemState* GameStateMision = Cast<AMisionSystemState>(UGameplayStatics::GetGameState(this));
       if (GameStateMision)
       {
          GameStateMision->OnMisionCompletada.AddDynamic(this, &ABridgeEvent::CanConstruct);
       }
    }
    
    const float TotalDistance = EndPointLocal.Size();
    if (TotalDistance > KINDA_SMALL_NUMBER && SegmentLength > 0.0f)
    {
       TotalSegments = FMath::CeilToInt(TotalDistance / SegmentLength);
       const FVector Direction = EndPointLocal.GetSafeNormal();
       BridgeRotation = Direction.Rotation();
       SegmentStepVector = Direction * (TotalDistance / TotalSegments);
    }
}

void ABridgeEvent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABridgeEvent::Interact_Implementation(AActor* Actor)
{
    IInteractInterface::Interact_Implementation(Actor);
    
    if (HordeManagerComponent)
    {
        HordeManagerComponent->StartHordeSystem();
    }
}

void ABridgeEvent::OnHordeCompleted()
{
    if (!HasAuthority()) return;
    StartBuilding();
}

void ABridgeEvent::StartBuilding()
{
    if (TotalSegments <= 0) return;
    
    SegmentsBuilt = 0;
    MeshBridgeInstance->ClearInstances();
    
    GetWorld()->GetTimerManager().SetTimer(BuildTimerHandle, this, &ABridgeEvent::BuildNexSegment, BuildStepInterval, true);
}

void ABridgeEvent::BuildNexSegment()
{
    SegmentsBuilt++;
    UpdateBridgeVisuals();
    
    if (SegmentsBuilt >= TotalSegments)
    {
       GetWorld()->GetTimerManager().ClearTimer(BuildTimerHandle);
       
       // El puente se terminó de armar: movemos el Respawn de todos los jugadores a la Zona 2
       ActualizarRespawnJugadores();

       OnBridgeCompleted.Broadcast();
    }
}

void ABridgeEvent::ActualizarRespawnJugadores()
{
    if (!HasAuthority()) return;

    const FTransform NuevoRespawn = PuntoRespawnPuente 
        ? PuntoRespawnPuente->GetComponentTransform() 
        : GetActorTransform();

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PC = It->Get())
        {
            if (AProyecto404TNFCharacter* PlayerChar = Cast<AProyecto404TNFCharacter>(PC->GetPawn()))
            {
                PlayerChar->SetRespawnTransform(NuevoRespawn);
            }
        }
    }
}

void ABridgeEvent::UpdateBridgeVisuals()
{
    while (MeshBridgeInstance->GetInstanceCount() < SegmentsBuilt)
    {
       const int32 IndexToBuild = MeshBridgeInstance->GetInstanceCount();
       const FVector SegmentLocation = SegmentStepVector * IndexToBuild;
       const FTransform SegmentTransform(BridgeRotation, SegmentLocation, FVector::OneVector);
       
       MeshBridgeInstance->AddInstance(SegmentTransform);
    }
}

void ABridgeEvent::OnRep_SegmentsBuilt()
{
    UpdateBridgeVisuals();
}