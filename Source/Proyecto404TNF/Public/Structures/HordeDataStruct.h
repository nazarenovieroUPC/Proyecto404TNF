#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HordeDataStruct.generated.h"


USTRUCT(BlueprintType)
struct FHordeWave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horde Definition")
	TSubclassOf<APawn> EnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horde Definition")
	int32 EnemiesToSpawn = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horde Definition")
	float SpawnInterval = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horde Definition")
	float WaveDuration = 60.0f; 
};