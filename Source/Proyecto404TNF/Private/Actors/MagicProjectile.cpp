// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MagicProjectile.h"

#include "Interfaces/DamageableInterface.h"


// Sets default values
AMagicProjectile::AMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComponent;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagicProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "DoDamage");
	
	if (OtherActor && OtherActor != GetOwner() && OtherActor -> Implements<UDamageableInterface>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "DoDamage");
		IDamageableInterface::Execute_TakeDamage(OtherActor, MagicDamage);
	} else
	{
		Destroy();
	}
}

// Called every frame
void AMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagicProjectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (Other)
	{
		Destroy();
	}
}


