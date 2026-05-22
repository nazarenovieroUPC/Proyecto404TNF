// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structures/ItemDataStruct.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (ClampMin = "1", ClampMax = "100", UIMin = "1", UIMax = "100"))
	int32 MaxInventorySlots = 20;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwapItems(int32 IndexA, int32 IndexB);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemAtIndex(int32 Index);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetItemAtIndex(int32 Index, FItemData ItemIn);
	
	UFUNCTION( BlueprintCallable, Category = "Inventory" )
	bool AddItem(FItemData NewItem);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void IncreaseInventorySize(int32 AdditionalSlots);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<FItemData> InventoryContents;
};
