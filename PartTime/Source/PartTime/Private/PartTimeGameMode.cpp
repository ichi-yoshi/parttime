#include "PartTimeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ItemDataTable.h"
#include "ItemActor.h"
#include "BasketActor.h"

void APartTimeGameMode::BeginPlay()
{
	Super::BeginPlay();

	// データベースからアイテムデータを読み込む
	TArray<FItemData> ItemDataList = ItemDataTable::ReadItemDataTable();
	ItemDataMap.Empty();
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

	TArray<FItemPatternModel> ItemPatternDataList = ItemDataTable::ReadItemPatternTable();
	ItemPatternDataMap.Empty();
	for (const FItemPatternModel& ItemPattern : ItemPatternDataList)
	{
		// マップにアイテムパターンデータを追加
		ItemPatternDataMap.Add(ItemPattern.BasketID, ItemPattern.ItemIDs);
	}

	TArray<AActor*> FoundBaskets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasketActor::StaticClass(), FoundBaskets);
	for(AActor* Actor : FoundBaskets)
	{
		if(ABasketActor* BasketActor = Cast<ABasketActor>(Actor))
		{
			if(const TArray<int32>* FoundItemIDs = ItemPatternDataMap.Find(BasketActor->BasketID))
			{
				// IDの配列を FItemData の配列に変換して渡す
				TArray<FItemData> TargetItems;
				for(int32 ID : *FoundItemIDs)
				{
					if(const FItemData* Data = ItemDataMap.Find(ID))
					{
						TargetItems.Add(*Data);
					}
				}

				BasketActor->SpawnItemsInBasket(TargetItems);
			}
		}
	}
}
