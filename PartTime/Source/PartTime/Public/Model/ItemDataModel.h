#pragma once

#include "CoreMinimal.h"
#include "ItemDataModel.generated.h"

USTRUCT(BlueprintType)
struct PARTTIME_API FItemData 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, category = "Item Data")
	int32 ID = 0;

	UPROPERTY(BlueprintReadOnly, category = "Item Data")
	FString ItemName;
	
	UPROPERTY(BlueprintReadOnly, category = "Item Data")
	float weight = 0.0f;

	UPROPERTY(BlueprintReadOnly, category = "Item Data")
	float Durability = 0.0f;
};
