#include "Actors/Projectiles/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/DamageableInterface.h"
#include "GameFramework/Pawn.h"

AProjectileBase::AProjectileBase()
{
    PrimaryActorTick.bCanEverTick = false;

    SetReplicates(true);
    SetReplicateMovement(true);

    InitialLifeSpan = 5.0f;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;
    SphereComponent->SetSphereRadius(16.0f);
    SphereComponent->SetCollisionProfileName(TEXT("Projectile"));
    
    SphereComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->InitialSpeed = 2200.0f;
    ProjectileMovementComponent->MaxSpeed = 2200.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f; 
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
}

bool AProjectileBase::CanDamageActor(AActor* Target) const
{
    if (!Target || Target == GetOwner() || Target == GetInstigator())
    {
        return false;
    }

    APawn* InstigatorPawn = GetInstigator();
    if (!InstigatorPawn)
    {
        InstigatorPawn = Cast<APawn>(GetOwner());
    }

    APawn* TargetPawn = Cast<APawn>(Target);

    if (InstigatorPawn && TargetPawn)
    {
        if (InstigatorPawn->IsPlayerControlled() && TargetPawn->IsPlayerControlled())
        {
            return false;
        }
        
        if (!InstigatorPawn->IsPlayerControlled() && !TargetPawn->IsPlayerControlled())
        {
            return false;
        }
    }

    return true;
}

void AProjectileBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    
    if (!HasAuthority()) return;

    if (OtherActor && OtherActor != GetOwner())
    {
        if (CanDamageActor(OtherActor))
        {
            if (OtherActor->Implements<UDamageableInterface>())
            {
                IDamageableInterface::Execute_TakeDamage(OtherActor, Damage, GetInstigator() ? Cast<AActor>(GetInstigator()) : GetOwner());
            }
            BP_OnImpact(GetActorLocation());
            Destroy();
        }
    }
}

void AProjectileBase::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                               bool bSelfMoved, FVector HitLocation, FVector HitNormal, 
                               FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    if (!HasAuthority()) return;

    if (Other && Other != GetOwner())
    {
        if (CanDamageActor(Other) && Other->Implements<UDamageableInterface>())
        {
            IDamageableInterface::Execute_TakeDamage(Other, Damage, GetInstigator() ? Cast<AActor>(GetInstigator()) : GetOwner());
        }
        
        BP_OnImpact(HitLocation);
        Destroy();
    }
}