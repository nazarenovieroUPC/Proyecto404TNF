// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LootBox.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "SkeletonTreeBuilder.h"
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
}





