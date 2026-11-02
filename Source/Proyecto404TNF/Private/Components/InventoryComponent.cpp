// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InventoryComponent.h"
#include "Actors/ItemBase.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    
    SetIsReplicatedByDefault(true);
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        InventoryContents.SetNum(MaxInventorySlots);
    }
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UInventoryComponent, InventoryContents);
    DOREPLIFETIME(UInventoryComponent, MonedasComida);
}

void UInventoryComponent::OnRep_InventoryContents()
{
    OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::OnRep_MonedasComida()
{
    OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::AddMonedas(int32 Cantidad)
{
    if (GetOwner() && GetOwner()->HasAuthority() && Cantidad > 0)
    {
        MonedasComida += Cantidad;
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, 
                FString::Printf(TEXT("¡Monedas sumadas! Total actual: %d"), MonedasComida));
        }
        OnRep_MonedasComida(); 
    }
}

bool UInventoryComponent::ConsumirMonedas(int32 Cantidad)
{
    if (GetOwner() && GetOwner()->HasAuthority() && Cantidad > 0 && MonedasComida >= Cantidad)
    {
        MonedasComida -= Cantidad;
        OnRep_MonedasComida();
        return true;
    }
    return false;
}

bool UInventoryComponent::AddItem(FItemData NewItem)
{
    for (int32 i = 0; i < InventoryContents.Num(); i++)
    {
        if (InventoryContents[i].Cantidad <= 0)
        {
            InventoryContents[i] = NewItem; 
            UE_LOG(LogTemp, Warning, TEXT("Item anadido: %s en el slot %d"), *NewItem.Nombre.ToString(), i);
            
            if (GetOwner() && GetOwner()->HasAuthority())
            {
                OnRep_InventoryContents();
            }
            return true; 
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Inventario Lleno!"));
    return false;
}

void UInventoryComponent::SwapItems(int32 IndexA, int32 IndexB)
{
    if (InventoryContents.IsValidIndex(IndexA) && InventoryContents.IsValidIndex(IndexB))
    {
        FItemData TempItem = InventoryContents[IndexA];
        InventoryContents[IndexA] = InventoryContents[IndexB];
        InventoryContents[IndexB] = TempItem;

        if (GetOwner() && GetOwner()->HasAuthority())
        {
            OnRep_InventoryContents();
        }
    }
}

void UInventoryComponent::RemoveItemAtIndex(int32 Index)
{
    SetItemAtIndex(Index, FItemData());
}

void UInventoryComponent::SetItemAtIndex(int32 Index, FItemData ItemIn)
{
    if (InventoryContents.IsValidIndex(Index))
    {
        InventoryContents[Index] = ItemIn;

        if (GetOwner() && GetOwner()->HasAuthority())
        {
            OnRep_InventoryContents();
        }
    }
}

void UInventoryComponent::Server_SoltarItem_Implementation(int32 Index)
{
    if (!InventoryContents.IsValidIndex(Index)) return;
    
    FItemData ItemToDrop = InventoryContents[Index];
    if (ItemToDrop.Cantidad <= 0 || !ItemToDrop.ClaseDelItem) return;
    
    AActor* OwnerActor = GetOwner();
    if (OwnerActor && OwnerActor->HasAuthority())
    {
        FVector SpawnLocation = OwnerActor->GetActorLocation() + (OwnerActor->GetActorForwardVector() * DropDistance);
        SpawnLocation.Z += 50.0f;
        FRotator SpawnRotation = OwnerActor->GetActorRotation();
       
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
       
        AActor* DroppedActor = GetWorld()->SpawnActor<AActor>(ItemToDrop.ClaseDelItem, SpawnLocation, SpawnRotation, SpawnParams);
       
        if (AItemBase* DroppedItem = Cast<AItemBase>(DroppedActor))
        {
            DroppedItem->SetItemData(ItemToDrop);
        }
        RemoveItemAtIndex(Index);
    }
}

void UInventoryComponent::IncreaseInventorySize(int32 AdditionalSlots)
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        MaxInventorySlots += AdditionalSlots;
        InventoryContents.SetNum(MaxInventorySlots);
        OnRep_InventoryContents();
    }
}

bool UInventoryComponent::RemoverItemPorNombre(FText NombreItem)
{
    for (int32 i = 0; i < InventoryContents.Num(); i++)
    {
        if (InventoryContents[i].Cantidad > 0 && InventoryContents[i].Nombre.EqualTo(NombreItem))
        {
            RemoveItemAtIndex(i);
            UE_LOG(LogTemp, Warning, TEXT("Se entrego el item: %s"), *NombreItem.ToString());
            return true;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("No se encontro el item: %s"), *NombreItem.ToString());
    return false;
}

int32 UInventoryComponent::ObtenerCantidadTotalPorNombre(FText NombreItem) const
{
    int32 Total = 0;
    const FString NombreBuscado = NombreItem.ToString();

    for (const FItemData& Slot : InventoryContents)
    {
        if (Slot.Nombre.ToString().Equals(NombreBuscado, ESearchCase::IgnoreCase))
        {
            Total += Slot.Cantidad;
        }
    }
    return Total;
}

bool UInventoryComponent::ConsumirItemPorNombre(FText NombreItem, int32 CantidadAConsumir)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || CantidadAConsumir <= 0)
    {
        return false;
    }
    
    if (ObtenerCantidadTotalPorNombre(NombreItem) < CantidadAConsumir)
    {
        return false;
    }

    int32 RestantePorQuitar = CantidadAConsumir;
    const FString NombreBuscado = NombreItem.ToString();
    
    for (int32 i = 0; i < InventoryContents.Num(); ++i)
    {
        if (InventoryContents[i].Nombre.ToString().Equals(NombreBuscado, ESearchCase::IgnoreCase))
        {
            if (InventoryContents[i].Cantidad <= RestantePorQuitar)
            {
                RestantePorQuitar -= InventoryContents[i].Cantidad;
                RemoveItemAtIndex(i);
            }
            else
            {
                InventoryContents[i].Cantidad -= RestantePorQuitar;
                RestantePorQuitar = 0;
                break;
            }

            if (RestantePorQuitar == 0)
            {
                break;
            }
        }
    }
    
    OnRep_InventoryContents();
    return true;
}