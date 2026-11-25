#pragma once

#include "CoreMinimal.h"
#include "ItemPatternModel.generated.h"

USTRUCT(BlueprintType)
struct  PARTTIME_API FItemPatternModel
{
	GENERATED_BODY()

	// バスケットのID
	UPROPERTY(BLueprintReadOnly, Category = "Item Pattern")
	FString BasketID;

	// アイテムIDの配列（最大10個）
	UPROPERTY(BlueprintReadOnly, Category = "Item Pattern")
	TArray<int32> ItemIDs;
};
