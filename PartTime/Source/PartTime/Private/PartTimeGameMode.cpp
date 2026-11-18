// Fill out your copyright notice in the Description page of Project Settings.


#include "PartTimeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ItemDataTable.h"
#include "ItemActor.h"

void APartTimeGameMode::BeginPlay()
{
	Super::BeginPlay();

	// データベースからアイテムデータを読み込む
	TArray<FItemData> ItemDataList = ItemDataTable::ReadItemDataTable();

	// アイテムIDをキー、FItemDataを値とするマップを作成
	TMap<int32, FItemData> ItemDataMap;
	for (const FItemData& ItemData : ItemDataList)
	{
		// マップにアイテムデータを追加
		ItemDataMap.Add(ItemData.ID, ItemData);
	}

	// ワールド内のすべてのAItemActorを取得し、対応するアイテムデータを適用
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemActor::StaticClass(), FoundActors);

	// 取得したAItemActorに対して、対応するアイテムデータを適用
	for(AActor* Actor : FoundActors)
	{
		// AItemActorにキャストして、アイテムIDに基づいてデータを適用
		AItemActor* ItemActor = Cast<AItemActor>(Actor);
		if(ItemActor)
		{
			// アイテムIDを取得し、マップから対応するアイテムデータを取得して適用
			int32 ItemID = ItemActor->ItemID;
			if(ItemDataMap.Contains(ItemID))
			{
				// マップから対応するアイテムデータを取得して、AItemActorに適用
				const FItemData& ItemData = ItemDataMap[ItemID];
				ItemActor->ApplyItemData(ItemData);
			}
		}
	}
}
