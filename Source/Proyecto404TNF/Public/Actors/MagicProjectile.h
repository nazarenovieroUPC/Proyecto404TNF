// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MagicProjectile.generated.h"

UCLASS()
class PROYECTO404TNF_API AMagicProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMagicProjectile();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Collision)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Collision)
	TObjectPtr<UMeshComponent> MeshComponent;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Stats)
	float MagicDamage = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
