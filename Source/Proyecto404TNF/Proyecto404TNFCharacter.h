// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InventoryComponent.h"
#include "Interfaces/DamageableInterface.h"
#include "Logging/LogMacros.h"
#include "Proyecto404TNFCharacter.generated.h"

class UCombatComponent;
class UHealthComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AProyecto404TNFCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	/** Dash Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	/** Interact Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	
	/** Attack Melee Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackMeleeAction;

public:
	AProyecto404TNFCharacter();
	
	//Health Component
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;
	
	//Combat Component
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UCombatComponent> CombatComponent;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UArrowComponent> ArrowMagicComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashForce = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashCoolDown = 0.4f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Damage = 15;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	/** Called for Dash input */
	void Dash(const FInputActionValue& Value);
	
	/** Called for Attack Melee input */
	void AttackMelee(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Overlap", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> OverlapActor;
	
	bool bCanDash = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	bool bCanAttack;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	//TIMERS
	FTimerHandle DashTimerHandle;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	//Overlap Events
	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION()
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	UFUNCTION(BlueprintCallable, Category = Health)
	virtual void TakeDamage_Implementation(float TakeDamage) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnDying();
	
	UFUNCTION(BLueprintCallable)
	void InteractOtherActor();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AnimationSwordAttack();
	
};

