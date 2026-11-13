// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/ItemDataModel.h"
#include "ItemActor.generated.h"

UCLASS()
class PARTTIME_API AItemActor : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID = 0;

	UFUNCTION(BlueprintNativeEvent,Category="Item")
	void ApplyItemData(const FItemData& NewData);
	virtual void ApplyItemData_Implementation(const FItemData& NewData);
};
