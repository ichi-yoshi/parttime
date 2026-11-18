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
	// エディタで指定したアイテムIDに基づいて、データベースから取得したアイテムデータを適用するための関数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID = 0;	// アイテムのIDを指定するためのプロパティ

	// データベースから取得したアイテムデータを適用するための関数
	UFUNCTION(BlueprintNativeEvent,Category="Item")
	void ApplyItemData(const FItemData& NewData);	// BlueprintNativeEventを使用して、Blueprintでオーバーライド可能にする
	virtual void ApplyItemData_Implementation(const FItemData& NewData);

	// アイテムデータを保持するためのプロパティ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FItemData ItemData;	// データベースから取得したアイテムデータを保持するためのプロパティ
};
