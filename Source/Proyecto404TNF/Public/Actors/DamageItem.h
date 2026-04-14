// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageItem.generated.h"

UCLASS()
class PROYECTO404TNF_API ADamageItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10.f;
	
	UFUNCTION(BlueprintCallable)
	void DoDamage(AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
