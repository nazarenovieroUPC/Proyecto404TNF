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
#include "Components/WidgetComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Widgets/BridgeInfoWidget.h"

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
    
    InfoWidgetComp = CreateDefaultSubobject<UWidgetComponent>("InfoWidgetComp");
    InfoWidgetComp->SetupAttachment(RootComponent);
    InfoWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
    InfoWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
    InfoWidgetComp->SetDrawAtDesiredSize(true);
    InfoWidgetComp->SetVisibility(false);
}

void ABridgeEvent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ABridgeEvent, bBridgeBuilt);
    DOREPLIFETIME(ABridgeEvent, SegmentsBuilt);
    DOREPLIFETIME(ABridgeEvent, Phase);
    DOREPLIFETIME(ABridgeEvent, HordeEndTime);
}

void ABridgeEvent::OnRep_BridgeBuilt()
{
    if (bBridgeBuilt && MeshBridge)
    {
       MeshBridge->SetVisibility(true);
       MeshBridge->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
       
       BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        
        RefreshInfoText();
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
    
    if (BoxCollision)
    {
        BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABridgeEvent::OnBoxBeginOverlap);
        BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABridgeEvent::OnBoxEndOverlap);
    }

    RefreshInfoText();
}

void ABridgeEvent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABridgeEvent::Interact_Implementation(AActor* Actor)
{
    IInteractInterface::Interact_Implementation(Actor);
    
    if (!HasAuthority() || !bBridgeBuilt || Phase != EBridgePhase::Idle) return;

    if (HordeManagerComponent)
    {
        if (const AGameStateBase* GS = GetWorld()->GetGameState())
        {
            HordeEndTime = GS->GetServerWorldTimeSeconds() + HordeManagerComponent->TotalSurvivalTime;
        }

        SetPhase(EBridgePhase::Horde);
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
    
    SetPhase(EBridgePhase::Building);
    
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
        
        SetPhase(EBridgePhase::Done);
       
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

void ABridgeEvent::OnRep_Phase()
{
    if (GetNetMode() == NM_DedicatedServer) return;

    if (Phase == EBridgePhase::Horde)
    {
        GetWorldTimerManager().SetTimer(InfoTimerHandle, this, &ABridgeEvent::RefreshInfoText, 0.25f, true);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(InfoTimerHandle);
    }

    RefreshInfoText();
}

void ABridgeEvent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (const AProyecto404TNFCharacter* PlayerChar = Cast<AProyecto404TNFCharacter>(OtherActor))
    {
        if (PlayerChar->IsLocallyControlled())
        {
            bLocalPlayerInside = true;
            RefreshInfoText();
        }
    }
}

void ABridgeEvent::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (const AProyecto404TNFCharacter* PlayerChar = Cast<AProyecto404TNFCharacter>(OtherActor))
    {
        if (PlayerChar->IsLocallyControlled())
        {
            bLocalPlayerInside = false;
            RefreshInfoText();
        }
    }
}

void ABridgeEvent::SetPhase(EBridgePhase NewPhase)
{
    Phase = NewPhase;
    OnRep_Phase();
}

void ABridgeEvent::RefreshInfoText()
{
    if (GetNetMode() == NM_DedicatedServer || !InfoWidgetComp) return;

    UBridgeInfoWidget* InfoWidget = Cast<UBridgeInfoWidget>(InfoWidgetComp->GetUserWidgetObject());
    if (!InfoWidget) return;

    FText Text;
    bool bShow = false;

    switch (Phase)
    {
    case EBridgePhase::Idle:
        if (bBridgeBuilt && bLocalPlayerInside)
        {
            Text = FText::FromString(TEXT("Presiona E para empezar a construir"));
            bShow = true;
        }
        break;

    case EBridgePhase::Horde:
        {
            const AGameStateBase* GS = GetWorld()->GetGameState();
            const float Remaining = GS ? HordeEndTime - GS->GetServerWorldTimeSeconds() : 0.f;

            if (Remaining > 0.f)
            {
                const int32 Secs = FMath::CeilToInt(Remaining);
                Text = FText::FromString(FString::Printf(TEXT("El puente se construye en %02d:%02d"), Secs / 60, Secs % 60));
            }
            else
            {
                Text = FText::FromString(TEXT("¡Elimina a los enemigos restantes!"));
            }
            bShow = true;
            break;
        }

    case EBridgePhase::Building:
        Text = FText::FromString(TEXT("Construyendo puente..."));
        bShow = true;
        break;

    case EBridgePhase::Done:
        break;
    }

    InfoWidget->SetInfoText(Text);
    InfoWidgetComp->SetVisibility(bShow);
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