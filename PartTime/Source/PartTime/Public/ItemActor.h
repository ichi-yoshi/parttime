// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/ItemDataModel.h"
#include "ItemActor.generated.h"

UCLASS()
class PARTTIME_API ItemActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ITemID = 0;

	UFUNCTION(BlueprintNative,Category="Item")
	void ApplyItemdata(const FItemData& NewData);
	virtual void ApplyItemData_Implementation(const FItemData& NewData);
};
