// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/EquipmentComponent.h"
#include "Net/UnrealNetwork.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipmentComponent, ReplicatedEquippedItems);
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

FItemData UEquipmentComponent::GetEquippedItemData(EEquipmentSlot SlotType)
{
	if (EquippedItems.Contains(SlotType))
	{
		return EquippedItems[SlotType];
	}
	return FItemData();
}

bool UEquipmentComponent::EquipItem(FItemData ItemToEquip)
{
	if (ItemToEquip.EquipmentSlot == EEquipmentSlot::None) return false;

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return false;
	
	if (!OwnerActor->HasAuthority())
	{
		EquippedItems.Add(ItemToEquip.EquipmentSlot, ItemToEquip);
		RecalculateStats();
		OnEquipmentChanged.Broadcast(ItemToEquip.EquipmentSlot, ItemToEquip);

		Server_EquipItem(ItemToEquip);
		return true;
	}
	
	EquippedItems.Add(ItemToEquip.EquipmentSlot, ItemToEquip);
	SyncReplicatedArray();
	RecalculateStats();
	OnEquipmentChanged.Broadcast(ItemToEquip.EquipmentSlot, ItemToEquip);

	return true;
}

void UEquipmentComponent::Server_EquipItem_Implementation(FItemData ItemToEquip)
{
	EquipItem(ItemToEquip);
}

bool UEquipmentComponent::UnequipItem(EEquipmentSlot SlotToUnequip)
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return false;

	if (!OwnerActor->HasAuthority())
	{
		if (!EquippedItems.Contains(SlotToUnequip)) return false;

		EquippedItems.Remove(SlotToUnequip);
		RecalculateStats();
		OnEquipmentChanged.Broadcast(SlotToUnequip, FItemData());

		Server_UnequipItem(SlotToUnequip);
		return true;
	}

	if (!EquippedItems.Contains(SlotToUnequip)) return false;

	EquippedItems.Remove(SlotToUnequip);
	SyncReplicatedArray();
	RecalculateStats();
	OnEquipmentChanged.Broadcast(SlotToUnequip, FItemData());

	return true;
}

void UEquipmentComponent::Server_UnequipItem_Implementation(EEquipmentSlot SlotToUnequip)
{
	UnequipItem(SlotToUnequip);
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
		const FItemData& Item = Pair.Value;

		for (const FStatModifier& Mod : Item.StatModifiers)
		{
			float& CurrentVal = CurrentStatBonuses.FindOrAdd(Mod.ItemStatType);
			CurrentVal += Mod.Value;
		}
	}
	
	OnStatsUpdated.Broadcast();
}

void UEquipmentComponent::SyncReplicatedArray()
{
	ReplicatedEquippedItems.Empty();
	for (const auto& Pair : EquippedItems)
	{
		ReplicatedEquippedItems.Add(Pair.Value);
	}
}

void UEquipmentComponent::OnRep_ReplicatedEquippedItems()
{
	TMap<EEquipmentSlot, FItemData> PreviousItems = EquippedItems;
	EquippedItems.Empty();

	for (const FItemData& Item : ReplicatedEquippedItems)
	{
		if (Item.EquipmentSlot != EEquipmentSlot::None)
		{
			EquippedItems.Add(Item.EquipmentSlot, Item);
			OnEquipmentChanged.Broadcast(Item.EquipmentSlot, Item);
		}
	}
	
	for (const auto& OldPair : PreviousItems)
	{
		if (!EquippedItems.Contains(OldPair.Key))
		{
			OnEquipmentChanged.Broadcast(OldPair.Key, FItemData());
		}
	}

	RecalculateStats();
}