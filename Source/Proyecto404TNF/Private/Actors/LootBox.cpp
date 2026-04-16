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
			float RandomX = FMath::RandRange(-40.f, 40.f);
			float RandomY = FMath::RandRange(-40.f, 40.f);
			float RandomZ = FMath::RandRange(20.f, 60.f); 
            
			FVector RandomOffset = FVector(RandomX, RandomY, RandomZ);
			FVector SpawnLocation = GetActorLocation() + RandomOffset;
			
			GetWorld()->SpawnActor<AActor>(LootToSpawn, GetActorLocation(),GetActorRotation(), SpawnParams);
		}	
	}
	
	Destroy();
}





