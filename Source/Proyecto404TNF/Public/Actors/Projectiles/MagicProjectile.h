// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "GameFramework/Actor.h"
#include "MagicProjectile.generated.h"

UCLASS()
class PROYECTO404TNF_API AMagicProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMagicProjectile();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Defaults")
	float MagicDamage = 0;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Defaults")
	
	TObjectPtr<AActor> CharacterInstigator;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	
};
