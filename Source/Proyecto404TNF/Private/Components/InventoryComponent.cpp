#include "Components/InventoryComponent.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InventoryContents.Reserve(InventorySlots);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::AddItem(FItemData NewItem)
{
	if (InventoryContents.Num() == InventorySlots)
	{
		return false;
	}
	
	InventoryContents.Add(NewItem);
	
	UE_LOG(LogTemp, Warning, TEXT("Item anadido: %s"), *NewItem.Nombre.ToString());
	return true;
}




