// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CombatComponent.h"
#include "Actors/Projectiles/MagicProjectile.h"
#include "Components/ArrowComponent.h"
#include "Interfaces/DamageableInterface.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetSystemLibrary.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

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

bool UCombatComponent::CanDamageActor(AActor* Target) const
{
    if (!Target || Target == GetOwner())
    {
        return false;
    }

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    APawn* TargetPawn = Cast<APawn>(Target);

    if (OwnerPawn && TargetPawn)
    {
        if (OwnerPawn->IsPlayerControlled() && TargetPawn->IsPlayerControlled())
        {
            return false;
        }
    	
        if (!OwnerPawn->IsPlayerControlled() && !TargetPawn->IsPlayerControlled())
        {
            return false;
        }
    }
	
    return true;
}

void UCombatComponent::MeleeAttack(float Damage)
{
    if (!GetOwner()->HasAuthority())
    {
       Server_MeleeAttack(Damage);
       return;
    }
    
    const FVector StartLocation = GetOwner()->GetActorLocation();
    const FVector ForwardVector = GetOwner()->GetActorForwardVector();
    const FVector EndLocation = StartLocation + (ForwardVector * 150.0f);
    
    ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
    
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());
    
    TArray<FHitResult> OutHits;
    
    bool bHit = UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(), StartLocation, EndLocation, 50.0f,
        TraceType, false, ActorsToIgnore, EDrawDebugTrace::None,
        OutHits, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
    
    if (bHit)
    {
       TSet<AActor*> ActoresGolpeados;

       for (const FHitResult& Hit : OutHits)
       {
          AActor* HitActor = Hit.GetActor();
          if (HitActor && !ActoresGolpeados.Contains(HitActor))
          {
             ActoresGolpeados.Add(HitActor);

             if (CanDamageActor(HitActor) && HitActor->Implements<UDamageableInterface>())
             {
                IDamageableInterface::Execute_TakeDamage(HitActor, Damage, GetOwner());
             }
          }
       }
    }
}

void UCombatComponent::MagicAttack(float MagicDamage, float MagicCoolDown)
{
    if (!GetOwner()->HasAuthority())
    {
       Server_MagicAttack(MagicDamage, MagicCoolDown);
       return;
    }
    
    if (bCanMagicAttack && Projectile)
    {
       FVector SpawnLocation = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 50.0f);
       FRotator SpawnRotation = GetOwner()->GetActorRotation();

       if (UArrowComponent* Arrow = GetOwner()->FindComponentByClass<UArrowComponent>())
       {
          SpawnLocation = Arrow->GetComponentLocation();
          SpawnRotation = Arrow->GetComponentRotation();
       }
       
       FActorSpawnParameters SpawnParams;
       SpawnParams.Owner = GetOwner();
       SpawnParams.Instigator = Cast<APawn>(GetOwner());
       
       AMagicProjectile* MagicProjectiles = GetWorld()->SpawnActor<AMagicProjectile>(Projectile, SpawnLocation, SpawnRotation, SpawnParams);
       if (MagicProjectiles)
       {
          MagicProjectiles->MagicDamage = MagicDamage;
          MagicProjectiles->CharacterInstigator = GetOwner();
       }
       
       OnMagicAttack.Broadcast();
       
       bCanMagicAttack = false;
       GetOwner()->GetWorldTimerManager().SetTimer(MagicTimerHandle, [this](){ bCanMagicAttack = true; }, MagicCoolDown, false);
    }
}

void UCombatComponent::RangedAttack(float Damage)
{
    if (!GetOwner()->HasAuthority())
    {
       Server_RangedAttack(Damage);
       return;
    }
    
    if (!Projectile) return;

    FVector SpawnLocation = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 50.0f);
    FRotator SpawnRotation = GetOwner()->GetActorRotation();

    if (UArrowComponent* Arrow = GetOwner()->FindComponentByClass<UArrowComponent>())
    {
       SpawnLocation = Arrow->GetComponentLocation();
       SpawnRotation = Arrow->GetComponentRotation();
    }
       
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.Instigator = Cast<APawn>(GetOwner());
       
    AProjectileBase* BaseProjectiles = GetWorld()->SpawnActor<AProjectileBase>(Projectile, SpawnLocation, SpawnRotation, SpawnParams);
    if (BaseProjectiles)
    {
       BaseProjectiles->Damage = Damage;
    }
}

bool UCombatComponent::Server_MeleeAttack_Validate(float Damage) { return true; }
void UCombatComponent::Server_MeleeAttack_Implementation(float Damage)
{
    MeleeAttack(Damage);
}

bool UCombatComponent::Server_MagicAttack_Validate(float MagicDamage, float MagicCoolDown) { return true; }
void UCombatComponent::Server_MagicAttack_Implementation(float MagicDamage, float MagicCoolDown)
{
    MagicAttack(MagicDamage, MagicCoolDown);
}

bool UCombatComponent::Server_RangedAttack_Validate(float Damage) { return true; }
void UCombatComponent::Server_RangedAttack_Implementation(float Damage)
{
    RangedAttack(Damage);
}