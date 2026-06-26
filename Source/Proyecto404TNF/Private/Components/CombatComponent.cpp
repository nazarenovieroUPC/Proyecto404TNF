// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Actors/Projectiles/MagicProjectile.h"
#include "Components/ArrowComponent.h"
#include "Interfaces/DamageableInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (GetOwner() && GetOwner()->GetWorldTimerManager().IsTimerActive(MagicTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(MagicTimerHandle);
	}
}

void UCombatComponent::MeleeAttack(float Damage)
{
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


void UCombatComponent::MagicAttack(float MagicDamage, float MagicCoolDown)
{
	if (bCanMagicAttack){
		
		UArrowComponent* Arrow = GetOwner()->FindComponentByClass<UArrowComponent>();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetOwner()->GetInstigator();
		
		AMagicProjectile* MagicProjectiles = GetWorld()->SpawnActor<AMagicProjectile>(Projectile, Arrow->GetComponentLocation(), Arrow->GetComponentRotation(), SpawnParams);
		if (MagicProjectiles)
		{
			MagicProjectiles -> MagicDamage = MagicDamage;
			
		}
		
		OnMagicAttack.Broadcast();
		
		bCanMagicAttack=false;
		GetOwner()->GetWorldTimerManager().SetTimer(MagicTimerHandle, [this](){bCanMagicAttack = true;}, MagicCoolDown, false);
	}
}

void UCombatComponent::RangedAttack(float Damage)
{
	UArrowComponent* Arrow = GetOwner()->FindComponentByClass<UArrowComponent>();
		
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner()->GetInstigator();
		
	AProjectileBase* BaseProjectiles = GetWorld()->SpawnActor<AProjectileBase>(Projectile, Arrow->GetComponentLocation(), Arrow->GetComponentRotation(), SpawnParams);
	if (BaseProjectiles){BaseProjectiles -> Damage = Damage;}
}

