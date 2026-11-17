// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DamageFlashComponent.h"

#include "GameFramework/Character.h"


// Sets default values for this component's properties
UDamageFlashComponent::UDamageFlashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
	
}

void UDamageFlashComponent::TriggerFlash()
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	
	Multicast_PlayFlash();
}

// Called when the game starts
void UDamageFlashComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GetNetMode() == NM_DedicatedServer) return;

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* Mesh = Character->GetMesh())
		{
			for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
			{
				if (UMaterialInstanceDynamic* MID = Mesh->CreateAndSetMaterialInstanceDynamic(i))
				{
					DynamicMaterials.Add(MID);
				}
			}
		}
	}
}


// Called every frame
void UDamageFlashComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	Elapsed += DeltaTime;

	if (Elapsed < WhiteDuration)
	{
		SetFlash(FLinearColor::White, FlashIntensity);
	}
	else
	{
		const float Alpha = (Elapsed - WhiteDuration) / RedDuration;

		if (Alpha >= 1.f)
		{
			SetFlash(FLinearColor::Black, 0.f);
			SetComponentTickEnabled(false);
			return;
		}

		SetFlash(FLinearColor::Red, FlashIntensity * (1.f - Alpha));
	}
}

void UDamageFlashComponent::SetFlash(const FLinearColor& Color, float Amount)
{
	for (UMaterialInstanceDynamic* MID : DynamicMaterials)
	{
		if (MID)
		{
			MID->SetVectorParameterValue(ColorParamName, Color);
			MID->SetScalarParameterValue(AmountParamName, Amount);
		}
	}
}

void UDamageFlashComponent::Multicast_PlayFlash_Implementation()
{
	if (DynamicMaterials.IsEmpty()) return;

	Elapsed = 0.f;
	SetComponentTickEnabled(true);
}

