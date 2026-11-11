// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ItemBase.h"
#include "Net/UnrealNetwork.h"
#include "Components/InventoryComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	SetReplicateMovement(true);
	
	MeshItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshItem"));
	RootComponent = MeshItem;
	
	MeshItem->SetSimulatePhysics(true);
	MeshItem->SetCollisionProfileName(TEXT("Custom"));
	MeshItem->SetCollisionResponseToAllChannels(ECR_Block);
	MeshItem->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeshItem->SetCollisionObjectType(ECC_PhysicsBody);
	MeshItem->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	MeshItem->SetLinearDamping(2.0f);
	MeshItem->SetAngularDamping(3.0f);
}

void AItemBase::SetItemData(const FItemData& NewData)
{
	if (HasAuthority())
	{
		DataDelItem = NewData;
	}
}


void AItemBase::Interact_Implementation(AActor* Interactor)
{
	if (Interactor)
	{
		UInventoryComponent* Inventario = Interactor->FindComponentByClass<UInventoryComponent>();
		if (Inventario)
		{
			bool bExito = Inventario->AddItem(DataDelItem);
			
			if (bExito)
			{
				if (HasAuthority())
				{
					Destroy();
				}
			}else
			{
				UE_LOG(LogTemp, Warning, TEXT("EL inventario esta lleno, no se recogio el item"));
			}
		}
	}
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AItemBase, DataDelItem);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

