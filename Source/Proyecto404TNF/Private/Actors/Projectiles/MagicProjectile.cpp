#include "Actors/Projectiles/MagicProjectile.h"

AMagicProjectile::AMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMagicProjectile::BeginPlay()
{
	if (MagicDamage > 0.0f)
	{
		Damage = MagicDamage;
	}

	Super::BeginPlay();
}