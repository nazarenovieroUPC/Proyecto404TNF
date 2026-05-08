// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structures/ItemDataStruct.h"
#include "EquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentSlot, Slot, FItemData, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatsUpdated);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UEquipmentComponent();

protected:
	
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipItem(FItemData ItemToEquip);
	
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnequipItem(EEquipmentSlot SlotToUnequip);
	
	UFUNCTION(BlueprintPure, Category = "Equipment/Stats")
	float GetStatBonus(EItemStatType StatType) const;
	
	UPROPERTY(BlueprintAssignable, Category = "Equipment/Events")
	FOnEquipmentChanged OnEquipmentChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Equipment/Events")
	FOnStatsUpdated OnStatsUpdated;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Equipment/State")
	TMap<EEquipmentSlot, FItemData> EquippedItems;
	
	UPROPERTY(VisibleAnywhere, Category = "Equipment/State")
	TMap<EItemStatType, float> CurrentStatBonuses;
	
	void RecalculateStats();
};
