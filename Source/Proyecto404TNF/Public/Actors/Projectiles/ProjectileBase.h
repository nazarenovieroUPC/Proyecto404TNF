// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	AProjectileBase();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Collision)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Collision)
	TObjectPtr<UMeshComponent> MeshComponent;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Stats)
	float Damage = 10;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	UFUNCTION()
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
