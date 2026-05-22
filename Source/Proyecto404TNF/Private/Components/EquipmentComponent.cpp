// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipmentComponent.h"


UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

FItemData UEquipmentComponent::GetEquippedItemData(EEquipmentSlot SlotType)
{
	if (EquippedItems.Contains(SlotType)){
		return EquippedItems[SlotType];
	}
	return FItemData();
}

bool UEquipmentComponent::EquipItem(FItemData ItemToEquip)
{
	if (ItemToEquip.EquipmentSlot == EEquipmentSlot::None) return false;
	
	EquippedItems.Add(ItemToEquip.EquipmentSlot, ItemToEquip);
	
	RecalculateStats();
	
	OnEquipmentChanged.Broadcast(ItemToEquip.EquipmentSlot, ItemToEquip);
	
	return true;
}

bool UEquipmentComponent::UnequipItem(EEquipmentSlot SlotToUnequip)
{
	if (!EquippedItems.Contains(SlotToUnequip)) return false;
	
	EquippedItems.Remove(SlotToUnequip);
	
	RecalculateStats();
	
	FItemData EmptyItem;
	OnEquipmentChanged.Broadcast(SlotToUnequip, EmptyItem);
	
	return true;
}

float UEquipmentComponent::GetStatBonus(EItemStatType StatType) const
{
	if (const float* FoundStat = CurrentStatBonuses.Find(StatType))
	{
		return *FoundStat;
	}
	return 0.0f;
}

void UEquipmentComponent::RecalculateStats()
{
	CurrentStatBonuses.Empty();
	
	for (const auto& Pair : EquippedItems)
	{
		FItemData Item = Pair.Value;
		
		for (const FStatModifier& Mod : Item.StatModifiers)
		{
			float& CurrentVal = CurrentStatBonuses.FindOrAdd(Mod.ItemStatType);
			CurrentVal += Mod.Value;
		}
	}
	OnStatsUpdated.Broadcast();
}
