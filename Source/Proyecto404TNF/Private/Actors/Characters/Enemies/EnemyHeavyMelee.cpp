// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Enemies/EnemyHeavyMelee.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Proyecto404TNF/Proyecto404TNFCharacter.h"


// Sets default values
AEnemyHeavyMelee::AEnemyHeavyMelee()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ChargeHitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ChargeHitBox"));
	ChargeHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ChargeHitBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyHeavyMelee::BeginPlay()
{
	Super::BeginPlay();
	
	ChargeHitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyHeavyMelee::OnChargeHit);
	
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyHeavyMelee::OnWallHit);
}

// Called every frame
void AEnemyHeavyMelee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyHeavyMelee::StartCharge()
{
	if (bIsCharging) return;
	
	bIsCharging = true;
	
	ChargeHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	GetCharacterMovement()->MaxWalkSpeed = SpeedCharge;
	
	GetWorldTimerManager().SetTimer(ChargeTimerHandle, this, &AEnemyHeavyMelee::StopCharge, CooldownCharge, false);
}

void AEnemyHeavyMelee::StopCharge()
{
	bIsCharging = false;
	
	ChargeHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeedChase;
	
	GetWorldTimerManager().ClearTimer(ChargeTimerHandle);
}

void AEnemyHeavyMelee::OnChargeHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (bIsCharging && OtherActor && OtherActor != this)
	{
		IDamageableInterface::Execute_TakeDamage(OtherActor, StatsComponent->StatsBase.MeleeDamage, GetOwner());

		if (AProyecto404TNFCharacter* HitCharacter = Cast<AProyecto404TNFCharacter>(OtherActor))
		{
			FVector PushDirection = GetActorForwardVector();
			PushDirection.Z += 0.3f;
			PushDirection.Normalize();
			
			FVector LaunchVelocity = PushDirection * KnockbackForceCharge;
			
			HitCharacter->LaunchCharacter(LaunchVelocity, true, true);
			
			StopCharge();
		}
	}
}

void AEnemyHeavyMelee::OnWallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsCharging && OtherActor && OtherActor != this && !Hit.GetActor()->Implements<AProyecto404TNFCharacter>())
	{
		StopCharge();
	}
}

// Called to bind functionality to input
void AEnemyHeavyMelee::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyHeavyMelee::EnemyAttack_Implementation()
{
	StartCharge();
}

