// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BridgeEvent.h"

#include "Components/BoxComponent.h"
#include "Components/HordeManagerComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABridgeEvent::ABridgeEvent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;
	BoxCollision->SetLineThickness(5);
	BoxCollision->SetBoxExtent(FVector(100,100,100));
	BoxCollision->SetHiddenInGame(false);
	
	MeshBridge = CreateDefaultSubobject<UStaticMeshComponent>("MeshLootBox");
	MeshBridge->SetupAttachment(RootComponent);
	
	HordeManagerComponent = CreateDefaultSubobject<UHordeManagerComponent>("HordeManagerComponent");
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

