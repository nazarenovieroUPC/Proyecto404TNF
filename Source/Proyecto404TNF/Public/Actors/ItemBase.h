// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structures/ItemDataStruct.h"
#include "Interfaces/InteractInterface.h"
#include "ItemBase.generated.h"

UCLASS()
class PROYECTO404TNF_API AItemBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "ItemData")
	FItemData DataDelItem;
	
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshItem;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
