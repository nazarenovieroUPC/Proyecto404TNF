// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LootComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/ItemBase.h"
#include "Components/LevelingComponent.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
ULootComponent::ULootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	XPToDrop = 50.0f;
	// ...
}


void ULootComponent::DropLoot()
{
    AActor* DuenoComponente = GetOwner();
    if (!DuenoComponente || !DuenoComponente->HasAuthority()) return;
    
    if (XPToDrop > 0.0f)
    {
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            if (APlayerController* PC = It->Get())
            {
                if (APawn* Pawn = PC->GetPawn())
                {
                    if (ULevelingComponent* PlayerLeveling = Pawn->FindComponentByClass<ULevelingComponent>())
                    {
                        PlayerLeveling->AddExperience(XPToDrop);
                    }
                }
            }
        }
    }
    
    if (ItemsDrop.Num() > 0)
    {
        DuenoComponente->SetActorEnableCollision(false);

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = nullptr;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        for (int32 i = 0; i < CantidadDrops; ++i)
        {
            int32 IndiceAleatorio = FMath::RandRange(0, ItemsDrop.Num() - 1);
            TSubclassOf<AItemBase> ItemAElegir = ItemsDrop[IndiceAleatorio];

            if (ItemAElegir)
            {
                FVector2D Offset2D = FMath::RandPointInCircle(RadioDispersion);
                FVector PosicionDrop = DuenoComponente->GetActorLocation() + FVector(Offset2D.X, Offset2D.Y, 60.0f);
                FRotator RotacionDrop = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

                AItemBase* NuevoItem = GetWorld()->SpawnActor<AItemBase>(ItemAElegir, PosicionDrop, RotacionDrop, SpawnParams);
                if (NuevoItem)
                {
                    if (UStaticMeshComponent* MeshComp = NuevoItem->FindComponentByClass<UStaticMeshComponent>())
                    {
                        MeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
                        MeshComp->AddImpulse(FVector(FMath::RandRange(-60.f, 60.f), FMath::RandRange(-60.f, 60.f), 200.0f), NAME_None, true);
                    }
                }
            }
        }
    }
}

// Called when the game starts
void ULootComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

