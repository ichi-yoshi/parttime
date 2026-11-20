// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "ItemZoneComponent.generated.h"

class AItemActor;

/**
カゴに付ける範囲。範囲内にある商品を返す
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PARTTIME_API UItemZoneComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UItemZoneComponent();

	// 範囲内にある商品を返す(商品の中心点が範囲内にある場合
	UFUNCTION(BlueprintCallable, Category = "ItemZone")
	TArray<AItemActor*> GetItemsInZone() const;
	
private:
	// ワールド座標の点が範囲内にあるか
	bool IsPointInZone(const FVector& WorldPoint) const;
};
