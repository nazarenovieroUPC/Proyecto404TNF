// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MisionSystemState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMisionCompletadaSignature);

UCLASS()
class PROYECTO404TNF_API AMisionSystemState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMisionSystemState();
	
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	
	UPROPERTY(BlueprintAssignable, Category = "Mision")
	FOnMisionCompletadaSignature OnMisionCompletada;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Mision")
	int32 ItemsRecolectados;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mision")
	int32 ItemsNecesarios;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Mision")
	bool bMisionActiva;
	
	UFUNCTION(BlueprintCallable, Category = "Mision")
	void AgregarItemMision();
};
