// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BridgeEvent.h"

#include "Components/BoxComponent.h"
#include "Components/HordeManagerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABridgeEvent::ABridgeEvent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	bPuenteConstruido = false;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;
	BoxCollision->SetLineThickness(5);
	BoxCollision->SetBoxExtent(FVector(100,100,100));
	BoxCollision->SetHiddenInGame(false);
	
	MeshBridge = CreateDefaultSubobject<UStaticMeshComponent>("MeshBridge");
	MeshBridge->SetupAttachment(RootComponent);
	
	MeshBridge->SetVisibility(false);
	MeshBridge->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	HordeManagerComponent = CreateDefaultSubobject<UHordeManagerComponent>("HordeManagerComponent");
}

void ABridgeEvent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABridgeEvent, bPuenteConstruido);
}

void ABridgeEvent::OnRep_PuenteConstruido()
{
	if (bPuenteConstruido)
	{
		MeshBridge->SetVisibility(true);
		MeshBridge->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("¡EL PUENTE SE HA CONSTRUIDO!"));
		}
	}
}

void ABridgeEvent::ConstruirPuente()
{
	if (HasAuthority())
	{
		bPuenteConstruido = true;
		OnRep_PuenteConstruido();
	}
}

// Called when the game starts or when spawned
void ABridgeEvent::BeginPlay()
{
	Super::BeginPlay();
	
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

