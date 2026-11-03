#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectiles/ProjectileBase.h"
#include "MagicProjectile.generated.h"

UCLASS()
class PROYECTO404TNF_API AMagicProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AMagicProjectile();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Magic")
	float MagicDamage = 25.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Magic")
	TObjectPtr<AActor> CharacterInstigator;

protected:
	virtual void BeginPlay() override;
};