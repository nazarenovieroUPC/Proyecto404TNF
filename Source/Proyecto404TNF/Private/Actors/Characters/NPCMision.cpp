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

void ANPCMision::EntregarMateriales(AActor* Jugador)
{
	if (!HasAuthority() || !Jugador) return;
	
	UInventoryComponent* Inventario = Jugador->FindComponentByClass<UInventoryComponent>();
	AMisionSystemState* GameStateMision = Cast<AMisionSystemState>(UGameplayStatics::GetGameState(this));

	if (Inventario && GameStateMision)
	{
		int32 MaderasEntregadas = 0;
		
		while (Inventario->RemoverItemPorNombre(NombreItemRequerido))
		{
			GameStateMision->AgregarItemMision();
			MaderasEntregadas++;
		}
		if (MaderasEntregadas > 0)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Entregaste %d de Madera"), MaderasEntregadas));
			
			if (GameStateMision->ItemsRecolectados >= GameStateMision->ItemsNecesarios)
			{
				if (PuenteVinculado)
				{
					PuenteVinculado->ConstruirPuente();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("ERROR: El NPC no tiene un puente asignado en sus variables."));
				}
			}
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No tenes madera en el inventario."));
		}
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

