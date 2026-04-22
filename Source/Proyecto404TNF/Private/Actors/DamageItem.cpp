// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DamageItem.h"

#include "Components/HealthComponent.h"
#include "Interfaces/DamageableInterface.h"

// Sets default values
ADamageItem::ADamageItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ADamageItem::DoDamage(AActor* OtherActor)
{
	IDamageableInterface::Execute_TakeDamage(OtherActor, Damage);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Se llama a la interfaz");
}

// Called when the game starts or when spawned
void ADamageItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADamageItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	DoDamage(OtherActor);
}

// Called every frame
void ADamageItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

