#include "Components/InventoryComponent.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InventoryContents.SetNum(MaxInventorySlots);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::SwapItems(int32 IndexA, int32 IndexB)
{
	if (InventoryContents.IsValidIndex(IndexA) && InventoryContents.IsValidIndex(IndexB))
	{
	FItemData TempItem = InventoryContents[IndexA];
	InventoryContents[IndexA] = InventoryContents[IndexB];
	InventoryContents[IndexB] = TempItem;
	}
}	

bool UInventoryComponent::AddItem(FItemData NewItem)
{
	for (int32 i = 0; i < InventoryContents.Num(); i++)
	{
		if (InventoryContents[i].Cantidad <= 0)
		{
			InventoryContents[i] = NewItem; 
			UE_LOG(LogTemp, Warning, TEXT("Item anadido: %s en el slot %d"), *NewItem.Nombre.ToString(), i);
			return true; 
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Inventario Lleno!"));
	return false;
}

void UInventoryComponent::IncreaseInventorySize(int32 AdditionalSlots)
{
	MaxInventorySlots += AdditionalSlots;
	InventoryContents.SetNum(MaxInventorySlots);
}




