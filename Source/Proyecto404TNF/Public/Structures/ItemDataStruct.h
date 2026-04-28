// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStruct.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
	struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FText Nombre;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FText Descripcion;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	TObjectPtr<UTexture2D> Icono;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	TSubclassOf<AActor> ClaseDelItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	int32 Cantidad;
};