// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MisionSystemState.generated.h"


UCLASS()
class PROYECTO404TNF_API AMisionSystemState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMisionSystemState();
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Mision")
	int32 ItemsRecolectados;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mision")
	int32 ItemsNecesarios;
	
	UFUNCTION(BlueprintCallable, Category = "Mision")
	void AgregarItemMision();
};
