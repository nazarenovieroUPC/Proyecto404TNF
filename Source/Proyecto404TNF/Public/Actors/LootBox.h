// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "LootBox.generated.h"

class UBoxComponent;

UCLASS()
class PROYECTO404TNF_API ALootBox : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	virtual void Interact_Implementation(AActor* Actor) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootBox", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LootBox", meta = (AllowPrivateAccess = true))
    	TObjectPtr<UStaticMeshComponent> MeshLootBox;
};
