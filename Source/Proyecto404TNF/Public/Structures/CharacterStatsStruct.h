#pragma once

#include "Engine/DataTable.h"
#include "CharacterStatsStruct.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatsRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float MaxHealth = 70.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Defense = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float MeleeDamage = 15.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float MagicDamage = 15.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Speed = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float MagicCoolDown = 5.f;
};
