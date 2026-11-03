// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExperienceChanged, float, CurrentXP, float, MaxXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUp, int32, NewLevel);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API ULevelingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULevelingComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category = "Leveling")
    void AddExperience(float XPToAdd);
    
    UFUNCTION(BlueprintPure, Category = "Leveling")
    float GetCurrentXP() const { return CurrentXP; }
    
    UFUNCTION(BlueprintPure, Category = "Leveling")
    float GetMaxXP() const { return MaxXP; }
    
    UFUNCTION(BlueprintPure, Category = "Leveling")
    int32 GetCurrentLevel() const { return CurrentLevel; }
    
    UPROPERTY(BlueprintAssignable, Category = "Leveling|Events")
    FOnExperienceChanged OnExperienceChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Leveling|Events")
    FOnLevelUp OnLevelUp;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentLevel, VisibleAnywhere, BlueprintReadOnly, Category = "Leveling|State")
    int32 CurrentLevel;

    UPROPERTY(ReplicatedUsing = OnRep_Experience, VisibleAnywhere, BlueprintReadOnly, Category = "Leveling|State")
    float CurrentXP;

    UPROPERTY(ReplicatedUsing = OnRep_Experience, VisibleAnywhere, BlueprintReadOnly, Category = "Leveling|State")
    float MaxXP;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Leveling|Config")
    float XPRequirementMultiplier;
    
    UFUNCTION()
    void OnRep_Experience();

    UFUNCTION()
    void OnRep_CurrentLevel(int32 OldLevel);

private:
    void CheckLevelUp();
};