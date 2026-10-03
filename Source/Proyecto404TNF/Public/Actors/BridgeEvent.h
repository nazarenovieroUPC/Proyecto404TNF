// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "BridgeEvent.generated.h"

class UBoxComponent;
class UHordeManagerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBridgeCompletedSignature);

UCLASS()
class PROYECTO404TNF_API ABridgeEvent : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABridgeEvent();
	
	UPROPERTY(BlueprintAssignable, Category = "Bridge|BuildingSystem")
	FOnBridgeCompletedSignature OnBridgeCompleted;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bridge|Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> StartPoint;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MeshBridge;
	
	//Bridge building
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInstancedStaticMeshComponent> MeshBridgeInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Setup", meta = (MakeEditWidget = true))
	FVector EndPointLocal = FVector(1000.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Setup")
	float SegmentLength = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Setup")
	float BuildStepInterval = 0.35f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UHordeManagerComponent> HordeManagerComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact_Implementation(AActor* Actor) override;
	
	UFUNCTION()
	void OnHordeCompleted();
	
	UFUNCTION()
	void StartBuilding();
	
private:
	UFUNCTION()
	void OnRep_SegmentsBuilt();
	
	UFUNCTION()
	void BuildNexSegment();
	
	UFUNCTION()
	void UpdateBridgeVisuals();
	
	UPROPERTY(ReplicatedUsing = OnRep_SegmentsBuilt)
	int32 SegmentsBuilt = 0;
	
	FTimerHandle BuildTimerHandle;
	
	int32 TotalSegments = 0;
	int32 CurrentSegmentIndex = 0;
	FRotator BridgeRotation;
	FVector SegmentStepVector;
};
