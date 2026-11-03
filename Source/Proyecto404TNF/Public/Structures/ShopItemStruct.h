#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ShopItemStruct.generated.h"

USTRUCT(BlueprintType)
struct FShopItemEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tienda")
	FDataTableRowHandle ItemHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tienda", meta = (ClampMin = "1"))
	int32 PrecioComida = 10;
};