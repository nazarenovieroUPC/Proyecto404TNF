// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LootBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALootBox::ALootBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Se presiono la E");

	if (LootToSpawn != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		int32 LootAmount = FMath::RandRange(MinLootAmount, MaxLootAmount);
		
		for (int32 i = 0; i < LootAmount; ++i)
		{
			float RandomX = FMath::RandRange(-10.f, 10.f);
			float RandomY = FMath::RandRange(-10.f, 10.f);
            
			FVector RandomOffset = FVector(RandomX, RandomY, 20.f);
			FVector SpawnLocation = GetActorLocation() + RandomOffset;
			
			AActor* SpawnedLoot = GetWorld()->SpawnActor<AActor>(LootToSpawn, SpawnLocation, GetActorRotation(), SpawnParams);
			
			if (SpawnedLoot)
			{
				UPrimitiveComponent* PhysicsComponent = Cast<UPrimitiveComponent>(SpawnedLoot->GetRootComponent());
                
				if (PhysicsComponent && PhysicsComponent->IsSimulatingPhysics())
				{
					float DirX = FMath::RandRange(-1.f, 1.f);
					float DirY = FMath::RandRange(-1.f, 1.f);
					float DirZ = FMath::RandRange(1.f, 2.f); 
                    
					FVector ImpulseDirection = FVector(DirX, DirY, DirZ).GetSafeNormal();
					
					float JumpForce = FMath::RandRange(300.f, 600.f); 
					
					PhysicsComponent->AddImpulse(ImpulseDirection * JumpForce, NAME_None, true);
				}
			
			}	
		}
	}
	Destroy();
}





