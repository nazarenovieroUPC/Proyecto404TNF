#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structures/ShopItemStruct.h"
#include "ShopComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTransaccionResultado, bool, bExito, FText, Mensaje);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UShopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UShopComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tienda|Catalogo")
	TArray<FShopItemEntry> CatalogoOfertas;

	UPROPERTY(BlueprintAssignable, Category = "Tienda|Eventos")
	FOnTransaccionResultado OnTransaccionResultado;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Tienda")
	void Server_ComprarItem(AActor* Comprador, int32 IndiceOferta);

	UFUNCTION(BlueprintCallable, Category = "Tienda")
	TArray<FShopItemEntry> GetCatalogo() const { return CatalogoOfertas; }
};