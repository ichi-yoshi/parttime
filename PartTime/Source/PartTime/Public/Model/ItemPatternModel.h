#pragma once

#include "CoreMinimal.h"
#include "ItemPatternModel.generated.h"

USTRUCT(BlueprintType)
struct  PARTTIME_API FItemPatternModel
{
	GENERATED_BODY()

	UPROPERTY(BLueprintReadOnly, Category = "Item Pattern")
	FString BasketID;

	UPROPERTY(BlueprintReadOnly, Category = "Item Pattern")
	TArray<int32> ItemIDs;
};
