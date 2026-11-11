#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class PROYECTO404TNF_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Damage = 10.0f;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool CanDamageActor(AActor* Target) const;

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, 
						   UPrimitiveComponent* OtherComp, bool bSelfMoved, 
						   FVector HitLocation, FVector HitNormal, 
						   FVector NormalImpulse, const FHitResult& Hit) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat|VFX")
	void BP_OnImpact(FVector ImpactLocation);
};