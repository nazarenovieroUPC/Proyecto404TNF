// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LootBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Actors/ItemBase.h"

// Sets default values
ALootBox::ALootBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	
	BoxCollision= CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;
	BoxCollision->SetLineThickness(5);
	BoxCollision->SetBoxExtent(FVector(100,100,100));
	BoxCollision->SetHiddenInGame(false);

	MeshLootBox = CreateDefaultSubobject<UStaticMeshComponent>("MeshLootBox");
	MeshLootBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALootBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALootBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALootBox::Interact_Implementation(AActor* Actor)
{
	if (HasAuthority() && !bEstaRota)
	{
		bEstaRota = true;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		for (const FLootDropConfig &Loot : LootList)
		{
			if (Loot.ItemClass == nullptr) continue;
		
			for (int32 i=0; i< Loot.Amount; ++i)
			{
				float RandomX = FMath::RandRange(-20.0f, 20.0f);
				float RandomY = FMath::RandRange(-20.0f, 20.0f);
			
				FVector RandomOffset = FVector(RandomX, RandomY, 20.0f);
				FVector SpawnLocation = GetActorLocation() + RandomOffset;
			
				AItemBase* SpawnedLoot = GetWorld() ->SpawnActor<AItemBase>(Loot.ItemClass, SpawnLocation, GetActorRotation(), SpawnParams);
			
				if (SpawnedLoot)
				{
					if (Loot.ItemData.DataTable != nullptr && !Loot.ItemData.RowName.IsNone())
					{
						FItemData* RowInfo = Loot.ItemData.DataTable->FindRow<FItemData>(Loot.ItemData.RowName, TEXT("LootSpawn"));
						if (RowInfo)
						{
							SpawnedLoot->DataDelItem = *RowInfo;
						}
					}
				
					UPrimitiveComponent* PhysicsComponent= Cast<UPrimitiveComponent>(SpawnedLoot->GetRootComponent());
				
					if (PhysicsComponent && PhysicsComponent->IsSimulatingPhysics())
					{
						float DirX = FMath::RandRange(-1.0f, 1.0f);
						float DirY = FMath::RandRange(-1.0f, 1.0f);
						float DirZ = FMath::RandRange(-1.0f, 2.0f);
					
						FVector ImpulseDirection = FVector(DirX, DirY, DirZ).GetSafeNormal();
						float JumpForce = FMath::RandRange(300.0f, 600.0f);
					
						PhysicsComponent->AddImpulse(ImpulseDirection * JumpForce, NAME_None, true);
					}
				}
			}
		}
		OnRep_CajaRota();
		SetLifeSpan(3.0f);
	}
}

void ALootBox::OnRep_CajaRota()
{
	if (bEstaRota)
	{
	}
}

void ALootBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALootBox, bEstaRota);
}





