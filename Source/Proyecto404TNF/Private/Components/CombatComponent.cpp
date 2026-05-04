// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Actors/MagicProjectile.h"
#include "Components/ArrowComponent.h"
#include "Interfaces/DamageableInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::MeleeAttack(float Damage)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Atacando");
	
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * 150);
	
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	
	TArray<FHitResult> OutHits;
	
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), StartLocation, EndLocation, 50.f,
		TraceType, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
		OutHits, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
	
	if (bHit)
	{
		for (const FHitResult& Hit: OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor -> Implements<UDamageableInterface>())
			{
				IDamageableInterface::Execute_TakeDamage(HitActor, Damage);
			}
		}
	}
}

void UCombatComponent::MagicAttack(float MagicDamage)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Magic Attack");
	UArrowComponent* Arrow = GetOwner()->FindComponentByClass<UArrowComponent>();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner()->GetInstigator();
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AMagicProjectile* MagicProjectiles = GetWorld()->SpawnActor<AMagicProjectile>(MagicProjectile, Arrow->GetComponentLocation(), Arrow->GetComponentRotation(), SpawnParams);
	if (MagicProjectiles)
	{
		MagicProjectiles -> MagicDamage = MagicDamage;
	}
}

