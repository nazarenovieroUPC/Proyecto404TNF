// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "BridgeEvent.generated.h"

class UBoxComponent;
class UHordeManagerComponent;
class UArrowComponent;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EBridgePhase : uint8
{
    Idle,
    Horde,
    Building,
    Done
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBridgeCompletedSignature);

UCLASS()
class PROYECTO404TNF_API ABridgeEvent : public AActor, public IInteractInterface
{
    GENERATED_BODY()

public:
    ABridgeEvent();
    
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    UPROPERTY(BlueprintAssignable, Category = "Bridge|BuildingSystem")
    FOnBridgeCompletedSignature OnBridgeCompleted;
    
    UPROPERTY(ReplicatedUsing = OnRep_BridgeBuilt, BlueprintReadOnly, Category = "Mision")
    bool bBridgeBuilt;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bridge|UI")
    TObjectPtr<UWidgetComponent> InfoWidgetComp;

    UFUNCTION()
    void OnRep_BridgeBuilt();
    
    UFUNCTION(BlueprintCallable, Category = "Mision")
    void CanConstruct();

protected:
    virtual void BeginPlay() override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bridge|Components", meta = (AllowPrivateAccess = true))
    TObjectPtr<USceneComponent> StartPoint;
       
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Components", meta = (AllowPrivateAccess = true))
    TObjectPtr<UBoxComponent> BoxCollision;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Components", meta = (AllowPrivateAccess = true))
    TObjectPtr<UStaticMeshComponent> MeshBridge;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Components", meta = (AllowPrivateAccess = true))
    TObjectPtr<UInstancedStaticMeshComponent> MeshBridgeInstance;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bridge|Respawn", meta = (AllowPrivateAccess = true))
    TObjectPtr<UArrowComponent> PuntoRespawnPuente;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Setup", meta = (MakeEditWidget = true))
    FVector EndPointLocal = FVector(1000.0f, 0.0f, 0.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Setup")
    float SegmentLength = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Setup")
    float BuildStepInterval = 0.35f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
    TObjectPtr<UHordeManagerComponent> HordeManagerComponent;

public:
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

    void ActualizarRespawnJugadores();
    
    UPROPERTY(ReplicatedUsing = OnRep_SegmentsBuilt)
    int32 SegmentsBuilt = 0;
    
    FTimerHandle BuildTimerHandle;
    
    int32 TotalSegments = 0;
    int32 CurrentSegmentIndex = 0;
    FRotator BridgeRotation;
    FVector SegmentStepVector;
    
    UPROPERTY(ReplicatedUsing = OnRep_Phase)
    EBridgePhase Phase = EBridgePhase::Idle;
    
    UPROPERTY(Replicated)
    float HordeEndTime = 0.f;

    UFUNCTION()
    void OnRep_Phase();

    UFUNCTION()
    void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex);

    void SetPhase(EBridgePhase NewPhase);
    void RefreshInfoText();

    FTimerHandle InfoTimerHandle;
    bool bLocalPlayerInside = false;
};