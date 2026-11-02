#pragma once

#include "CoreMinimal.h"
#include "Actors/ItemBase.h"
#include "ItemMoneda.generated.h"

UCLASS()
class PROYECTO404TNF_API AItemMoneda : public AItemBase
{
	GENERATED_BODY()

public:
	AItemMoneda();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnetismo")
	float MagnetRadius = 450.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnetismo")
	float FlySpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnetismo")
	float PickupDistance = 65.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnetismo")
	float RetardoAtraccion = 0.25f;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	FTimerHandle TimerDeteccion;

	void IniciarBusqueda();
	void BuscarJugadorCercano();
};