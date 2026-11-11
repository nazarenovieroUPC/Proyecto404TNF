// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structures/CharacterStatsStruct.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTotalStatsUpdated);

class UEquipmentComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UStatsComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    UPROPERTY(ReplicatedUsing = OnRep_StatsTotal, BlueprintReadOnly, Category = "Stats")
    FCharacterStatsRow StatsTotal;

    UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
    FOnTotalStatsUpdated OnTotalStatsUpdated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    TObjectPtr<UDataTable> StatsDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FName LevelRowID;
    
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FCharacterStatsRow StatsBase;
    
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FCharacterStatsRow StatsBuff;

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void ChangeSpeedMovement(float Speed);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void ChangeLevelRowID(int32 ID);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void OnBuff();

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void InitializeEquipmentLink(UEquipmentComponent* EquipComp);
    
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void RecalculateTotalStats();

protected:
    virtual void BeginPlay() override;
    
    UFUNCTION()
    void OnRep_StatsTotal();

private:
    UPROPERTY()
    TObjectPtr<UEquipmentComponent> CachedEquipment;
};