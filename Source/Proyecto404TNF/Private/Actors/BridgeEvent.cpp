// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BridgeEvent.h"
#include "Components/BoxComponent.h"
#include "Components/HordeManagerComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameStates/MisionSystemState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABridgeEvent::ABridgeEvent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	bBridgeBuilt = false;
	
	StartPoint = CreateDefaultSubobject<USceneComponent>("RootComp");
	SetRootComponent(StartPoint);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetLineThickness(5);
	BoxCollision->SetBoxExtent(FVector(100,100,100));
	BoxCollision->SetHiddenInGame(false);
	
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	MeshBridge = CreateDefaultSubobject<UStaticMeshComponent>("MeshBridge");
	MeshBridge->SetupAttachment(RootComponent);
	
	MeshBridge->SetVisibility(false);
	MeshBridge->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	MeshBridgeInstance = CreateDefaultSubobject<UInstancedStaticMeshComponent>("MeshBridgeInstance");
	MeshBridgeInstance->SetupAttachment(RootComponent);
	
	MeshBridgeInstance->bNavigationRelevant = true;
	
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
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("¡EL PUENTE SE HA CONSTRUIDO!"));
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

// Called when the game starts or when spawned
void ABridgeEvent::BeginPlay()
{
	Super::BeginPlay();
	
	if (HordeManagerComponent) HordeManagerComponent->OnHordeVictory.AddDynamic(this, &ABridgeEvent::OnHordeCompleted);
	
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

// Called every frame
void ABridgeEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABridgeEvent::Interact_Implementation(AActor* Actor)
{
	IInteractInterface::Interact_Implementation(Actor);
	
	HordeManagerComponent->StartHordeSystem();
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
		OnBridgeCompleted.Broadcast();
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