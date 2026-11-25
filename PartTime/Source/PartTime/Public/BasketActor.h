#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.h"
#include "Model/ItemDataModel.h"
#include "BasketActor.generated.h"

UCLASS()
class PARTTIME_API ABasketActor : public AActor
{
	GENERATED_BODY()

public:
	ABasketActor();

	// 10個分の配置オフセットを初期化する
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basket", meta = (ExposeOnSpawn = "true"))
	FString BasketID = TEXT("A");

	// アイテムのブループリントが格納されているフォルダのパス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basket")
	FString ItemBlueprintFolder = TEXT("/Game/Aseet/Syouhin");

	// アイテムを配置するためのオフセット配列
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basket")
	TArray<FVector> ItemSlotOffsets;

	// アイテムをバスケットにスポーンする
	UFUNCTION(BlueprintCallable, Category = "Basket")
	void SpawnItemsInBasket(const TArray<FItemData>& ItemsToSpawn);

	// バスケット内のアイテムをすべて削除する
	UFUNCTION(BlueprintCallable, Category = "Basket")
	void ClearBasket();

protected:
	// バスケットのメッシュコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basket")
	UStaticMeshComponent* BasketMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basket")
	TArray<USceneComponent*> ItemSlots;
private:
	UPROPERTY()
	TArray<AItemActor*> SpawnedItemActors;
};
