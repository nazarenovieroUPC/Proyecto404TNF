// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "NPCMision.generated.h"

class ABridgeEvent;

UCLASS()
class PROYECTO404TNF_API ANPCMision : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANPCMision();
	
	virtual void Interact_Implementation(AActor* Interactor) override;
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Mision")
	void Server_AceptarMision();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Mision")
	void Server_EntregarMateriales(AActor* Jugador);
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mision")
	TObjectPtr<ABridgeEvent> PuenteVinculado;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mision")
	FText NombreItemRequerido;
	
	UPROPERTY(EditDefaultsOnly, Category = "Mision UI")
	TSubclassOf<class UWidgetMision> ClaseWidgetMision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> MeshNPC;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
