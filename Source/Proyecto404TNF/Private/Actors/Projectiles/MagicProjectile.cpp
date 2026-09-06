// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/MagicProjectile.h"

#include "Interfaces/DamageableInterface.h"


// Sets default values
AMagicProjectile::AMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagicProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor && OtherActor != GetOwner() && OtherActor -> Implements<UDamageableInterface>())
	{
		IDamageableInterface::Execute_TakeDamage(OtherActor, MagicDamage, CharacterInstigator);
	} else
	{
		Destroy();
	}
}


