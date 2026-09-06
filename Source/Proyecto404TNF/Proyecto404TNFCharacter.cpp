// Copyright Epic Games, Inc. All Rights Reserved.

#include "Proyecto404TNFCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Actors/LootBox.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "Interfaces/InteractInterface.h"
#include "Components/LevelingComponent.h"
#include "Components/EquipmentComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProyecto404TNFCharacter

AProyecto404TNFCharacter::AProyecto404TNFCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	//Set Leveling Component
	LevelingComponent = CreateDefaultSubobject<ULevelingComponent>("LevelingComponent");
	
	//set Inventory Component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	//Set Equipment Component
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");
	
	//Set Health Component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	
	//Set Combat Component
	CombatComponent = CreateDefaultSubobject<UCombatComponent>("CombatComponent");
	
	//Set Stats Component
	StatsComponent = CreateDefaultSubobject<UStatsComponent>("StatsComponent");
}

void AProyecto404TNFCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	if (HealthComponent){ HealthComponent -> OnDeath.AddDynamic(this, &AProyecto404TNFCharacter::OnDying); }
	
	if (StatsComponent && EquipmentComponent)
	{
		StatsComponent->InitializeEquipmentLink(EquipmentComponent);
		
		StatsComponent->OnTotalStatsUpdated.AddDynamic(this, &AProyecto404TNFCharacter::SyncStatsWithComponents);
	}
}

//Overlap Events
void AProyecto404TNFCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	OverlapActor = OtherActor;
}

void AProyecto404TNFCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	OverlapActor = nullptr;
}

//Damage Interface
void AProyecto404TNFCharacter::TakeDamage_Implementation(float TakeDamage, AActor* DamagerActor)
{
	IDamageableInterface::TakeDamage_Implementation(TakeDamage, DamagerActor);
	if (HealthComponent) { HealthComponent -> HandleDamage(TakeDamage); }
}

void AProyecto404TNFCharacter::OnDying()
{
	Destroy();
}

void AProyecto404TNFCharacter::InteractOtherActor()
{
	ALootBox* CajaDetectada = Cast<ALootBox>(OverlapActor);
	
	if (CajaDetectada)
	{
		if (!HasAuthority())
		{
			ServerRomperCaja(CajaDetectada);
		}
		else
		{
			CajaDetectada->Interact_Implementation(this);
		}
	}
	else if (OverlapActor && OverlapActor->Implements<UInteractInterface>())
	{
		
		if (!HasAuthority())
		{
			Server_InteractuarGenerico(OverlapActor);
		}
		else
		{
			IInteractInterface::Execute_Interact(OverlapActor, this);
		}
	}
}

void AProyecto404TNFCharacter::SyncStatsWithComponents()
{
	if (HealthComponent && StatsComponent)
	{
		HealthComponent->UpdateMaxHealth(StatsComponent->StatsTotal.MaxHealth);
	}
}

void AProyecto404TNFCharacter::Server_InteractuarGenerico_Implementation(AActor* ActorInteractuable)
{
	if (ActorInteractuable && ActorInteractuable->Implements<UInteractInterface>())
	{
		IInteractInterface::Execute_Interact(ActorInteractuable, this);
	}	
}

void AProyecto404TNFCharacter::ServerRomperCaja_Implementation(ALootBox* CajaAtacada)
{
	if (CajaAtacada)
	{
		CajaAtacada->Interact_Implementation(this);
	}
}

void AProyecto404TNFCharacter::AnimationSwordAttack_Implementation()
{
}


//////////////////////////////////////////////////////////////////////////
// Input

void AProyecto404TNFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		//Interact
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AProyecto404TNFCharacter::Dash);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProyecto404TNFCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProyecto404TNFCharacter::Look);
		
		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AProyecto404TNFCharacter::InteractOtherActor);
		
		//Attack Melee
		EnhancedInputComponent->BindAction(AttackMeleeAction, ETriggerEvent::Started, this, &AProyecto404TNFCharacter::AttackMelee);
		
		//Attack Magic
		EnhancedInputComponent->BindAction(AttackMagicAction, ETriggerEvent::Started, this, &AProyecto404TNFCharacter::AttackMagic);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProyecto404TNFCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AProyecto404TNFCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProyecto404TNFCharacter::Dash(const FInputActionValue& Value)
{
	if (Controller != nullptr && bCanDash)
	{
		FVector DashDirection = GetActorForwardVector();
		
		LaunchCharacter(DashForce * DashDirection, true, true);
		
		bCanDash = false;
		
		GetWorldTimerManager().SetTimer(DashTimerHandle, [this](){bCanDash = true;}, DashCoolDown, false);
	}
}

//Attacks
void AProyecto404TNFCharacter::AttackMelee(const FInputActionValue& Value)
{
	if (bCanAttack){
		bCanAttack = false;
		
		AnimationSwordAttack();
		
		if (CombatComponent){ CombatComponent -> MeleeAttack(StatsComponent->StatsTotal.MeleeDamage); }
	}
}

void AProyecto404TNFCharacter::AttackMagic(const FInputActionValue& Value)
{
	if (CombatComponent){ CombatComponent -> MagicAttack(StatsComponent->StatsTotal.MagicDamage, StatsComponent->StatsTotal.MagicCoolDown);}
}
