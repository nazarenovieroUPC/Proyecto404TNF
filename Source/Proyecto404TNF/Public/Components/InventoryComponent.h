// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structures/ItemDataStruct.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();
    
    UPROPERTY(BlueprintAssignable, Category = "Inventory|Events")
    FOnInventoryUpdated OnInventoryUpdated;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    UPROPERTY(ReplicatedUsing = OnRep_MonedasComida, VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Economia")
    int32 MonedasComida = 0;

    UFUNCTION()
    void OnRep_MonedasComida();

    UFUNCTION(BlueprintCallable, Category = "Inventory|Economia")
    void AddMonedas(int32 Cantidad);

    UFUNCTION(BlueprintCallable, Category = "Inventory|Economia")
    bool ConsumirMonedas(int32 Cantidad);
    
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(FItemData NewItem);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SwapItems(int32 IndexA, int32 IndexB);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RemoveItemAtIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SetItemAtIndex(int32 Index, FItemData ItemIn);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
    void Server_SoltarItem(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void IncreaseInventorySize(int32 AdditionalSlots);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoverItemPorNombre(FText NombreItem);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 ObtenerCantidadTotalPorNombre(FText NombreItem) const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool ConsumirItemPorNombre(FText NombreItem, int32 CantidadAConsumir);

protected:
    virtual void BeginPlay() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Config", meta = (ClampMin = "1", ClampMax = "100", UIMin = "1", UIMax = "100"))
    int32 MaxInventorySlots = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Config")
    float DropDistance = 150.0f;
    
    UPROPERTY(ReplicatedUsing = OnRep_InventoryContents, VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FItemData> InventoryContents;

    UFUNCTION()
    void OnRep_InventoryContents();
};