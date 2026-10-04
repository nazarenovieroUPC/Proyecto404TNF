// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/NPCMision.h"
#include "Actors/BridgeEvent.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/WidgetMision.h"
#include "Components/InventoryComponent.h"
#include "GameStates/MisionSystemState.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANPCMision::ANPCMision()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshNPC = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshNPC"));
	RootComponent = MeshNPC;
	
	NombreItemRequerido = FText::FromString("Madera");
}

void ANPCMision::Interact_Implementation(AActor* Interactor)
{
	APawn* JugadorPawn = Cast<APawn>(Interactor);
	if (!JugadorPawn) return;
	
	APlayerController* PC = Cast<APlayerController>(JugadorPawn->GetController());
	
	if (PC && PC->IsLocalController() && ClaseWidgetMision)
	{
		UWidgetMision* WidgetMenu = CreateWidget<UWidgetMision>(PC, ClaseWidgetMision);
		
		if (WidgetMenu)
		{
			WidgetMenu->NPCVinculado = this;
			WidgetMenu->AddToViewport();
			
			FInputModeUIOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
		}
	}
}

void ANPCMision::Server_EntregarMateriales_Implementation(AActor* Jugador)
{
	AMisionSystemState* GameStateMision = Cast<AMisionSystemState>(UGameplayStatics::GetGameState(this));
	
	if (!GameStateMision || !GameStateMision->bMisionActiva)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("La misión aún no ha sido aceptada."));
		return;
	}

	UInventoryComponent* Inventario = Jugador->FindComponentByClass<UInventoryComponent>();

	if (Inventario)
	{
		int32 MaderasEntregadas = 0;
		
		while (Inventario->RemoverItemPorNombre(NombreItemRequerido))
		{
			GameStateMision->AgregarItemMision();
			MaderasEntregadas++;
		}
	}
}

void ANPCMision::Server_AceptarMision_Implementation()
{
	AMisionSystemState* GameStateMision = Cast<AMisionSystemState>(UGameplayStatics::GetGameState(this));
    
	// Si la mision no estaba activa, la activamos para todos
	if (GameStateMision && !GameStateMision->bMisionActiva)
	{
		GameStateMision->bMisionActiva = true;
        
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("¡LA MISIÓN HA COMENZADO PARA TODOS!"));
	}
}

// Called when the game starts or when spawned
void ANPCMision::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPCMision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

