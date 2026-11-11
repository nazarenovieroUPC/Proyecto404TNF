// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTO404TNF_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public: 
    UHealthComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealth() const { return ActualHealth; }
    
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const { return MaxHealth; }
    
    UFUNCTION(BlueprintPure, Category = "Health")
    bool GetIsDead() const { return bIsDead; }
    
    UFUNCTION(BlueprintCallable, Category = "Health")
    void HandleDamage(float Damage);
    
    UFUNCTION(BlueprintCallable, Category = "Health")
    void HandleHeal(float Heal);
    
    UFUNCTION(BlueprintCallable, Category = "Health")
    void UpdateMaxHealth(float NewMaxHealth);
    
    UPROPERTY(BlueprintAssignable, Category = "Health|Events")
    FOnDeath OnDeath;
    
    UPROPERTY(BlueprintAssignable, Category = "Health|Events")
    FOnHealthChanged OnHealthChanged;
    
    UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, EditAnywhere, Category = "Health")
    float MaxHealth = 100.f;
    
    UPROPERTY(ReplicatedUsing = OnRep_ActualHealth, BlueprintReadOnly, EditAnywhere, Category = "Health")
    float ActualHealth = 100.f;
    
    UPROPERTY(ReplicatedUsing = OnRep_IsDead, BlueprintReadOnly, EditAnywhere, Category = "Health")
    bool bIsDead = false;

protected:
    virtual void BeginPlay() override;
    
    UFUNCTION()
    void OnRep_ActualHealth();

    UFUNCTION()
    void OnRep_MaxHealth();
    
    UFUNCTION()
    void OnRep_IsDead();

private:
    void HandleDeath();
};