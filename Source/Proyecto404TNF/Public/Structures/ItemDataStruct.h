// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStruct.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	None UMETA(DisplayName = "Ninguno / Consumible"),
	Helmet UMETA(DisplayName = "Casco"),
	Chest UMETA(DisplayName = "Pechera"),
	Boots UMETA(DisplayName = "Botas"),
	Weapon UMETA(DisplayName = "Arma"),
	Pet UMETA(DisplayName = "Mascota")
};

UENUM(BlueprintType)
enum class EItemStatType : uint8
{
	Health UMETA(DisplayName = "Vida"),
	Damage UMETA(DisplayName = "Daño"),
	Defense UMETA(DisplayName = "Defensa"),
	Speed UMETA(DisplayName = "Velocidad")
};

USTRUCT(BlueprintType)
struct FStatModifier
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemStatType ItemStatType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;
};
	
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
	int32 Cantidad = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData/Equipment")
	EEquipmentSlot EquipmentSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData/Stats")
	TArray<FStatModifier> StatModifiers;
};