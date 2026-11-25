#pragma once

#include "CoreMinimal.h"
#include "ItemDataModel.generated.h"

USTRUCT(BlueprintType)
struct PARTTIME_API FItemData 
{
	GENERATED_BODY()

	// アイテムのID
	UPROPERTY(BlueprintReadOnly, category = "Item Data")
	int32 ID = 0;

	// アイテムの名前
	UPROPERTY(BlueprintReadOnly, category = "Item Data")
	FString ItemName;
	
	// アイテムの価格
	UPROPERTY(BlueprintReadOnly, category = "Item Data")
	float weight = 0.0f;

	// アイテムの耐久度
	UPROPERTY(BlueprintReadOnly, category = "Item Data")
	float Durability = 0.0f;
};
