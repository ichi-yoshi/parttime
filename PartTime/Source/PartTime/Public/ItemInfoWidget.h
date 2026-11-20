// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Model/ItemDataModel.h"
#include "ItemInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PARTTIME_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void ShowItemInfo(const FItemData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void HideItemInfo();
};
