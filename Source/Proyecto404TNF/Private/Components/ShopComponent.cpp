#include "Components/ShopComponent.h"
#include "Components/InventoryComponent.h"
#include "Structures/ItemDataStruct.h"

UShopComponent::UShopComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UShopComponent::Server_ComprarItem_Implementation(AActor* Comprador, int32 IndiceOferta)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || !Comprador) return;

    if (!CatalogoOfertas.IsValidIndex(IndiceOferta))
    {
        OnTransaccionResultado.Broadcast(false, FText::FromString(TEXT("Oferta no válida.")));
        return;
    }

    const FShopItemEntry& Oferta = CatalogoOfertas[IndiceOferta];

    if (Oferta.ItemHandle.IsNull())
    {
        OnTransaccionResultado.Broadcast(false, FText::FromString(TEXT("El ítem no está configurado.")));
        return;
    }

    FItemData* DatosItemComprado = Oferta.ItemHandle.GetRow<FItemData>(TEXT("Shop_GetRow"));
    if (!DatosItemComprado)
    {
        OnTransaccionResultado.Broadcast(false, FText::FromString(TEXT("No se encontró la fila en la tabla de datos.")));
        return;
    }

    UInventoryComponent* Inventario = Comprador->FindComponentByClass<UInventoryComponent>();
    if (!Inventario) return;

    // 1. Validar fondos
    if (Inventario->MonedasComida < Oferta.PrecioComida)
    {
        OnTransaccionResultado.Broadcast(false, FText::FromString(TEXT("No tienes suficiente comida.")));
        return;
    }

    // 2. Preparar el ítem
    FItemData ItemAEntregar = *DatosItemComprado;
    if (ItemAEntregar.Cantidad <= 0) ItemAEntregar.Cantidad = 1;

    // 3. Probar si entra en el inventario
    if (!Inventario->AddItem(ItemAEntregar))
    {
        OnTransaccionResultado.Broadcast(false, FText::FromString(TEXT("Inventario lleno.")));
        return;
    }

    // 4. Cobrar de la billetera
    Inventario->ConsumirMonedas(Oferta.PrecioComida);
    OnTransaccionResultado.Broadcast(true, FText::FromString(TEXT("¡Compra realizada con éxito!")));
}