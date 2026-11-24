#pragma once
#include "CoreMinimal.h"
#include "Model/ItemDataModel.h"
#include "SQLiteReader.h"
#include "Misc/Paths.h"

class PARTTIME_API ItemDataTable
{
public:
	// データベースからアイテムデータを読み込む静的関数
	static TArray<FItemData> ReadItemDataTable();

private:
	template <typename TModel, typename TMapper>
	static TArray<TModel> LoadTableData(const FString& TableName)
	{
		TArray<TModel> DataList;

		// データベースのパスを指定
		FString DbPath = FPaths::ProjectContentDir() / TEXT("DataBase/ItemData.db");

		// SQLクエリを作成して、Itemテーブルから全てのデータを取得
		FString Query = FString::Printf(TEXT("SELECT * FROM %s"), *TableName);

		TMapper Mapper;

		bool bSuccess = FSQLiteGenericReader::ReadTable<FItemData>(DbPath, Query, Mapper, DataList);

		// デバッグ用に読み込んだアイテムデータをログに出力
		if(bSuccess)
		{
			// 読み込んだアイテムデータをログに出力
			for(const FItemData& Item : DataList)
			{
				UE_LOG(LogTemp, Log, TEXT("ID: %d, Name: %s, Weight: %f,Durability: %f"),
					Item.ID, *Item.ItemName, Item.weight, Item.Durability);
			}

			//for(const FItemPatternData& ItemPattern : DataList) 
			//{
			//	// ItemIDs配列をカンマ区切りの文字列に変換
			//	FString ItemIDsStr = FString::JoinBy(Pattern, ItemIDs, TEXT(","), ), [](int32 ItemID) 
			//	{
			//		return FString::FromInt(ItemID); 
			//	});
			//	UE_LOG(LogTemp, Log, TEXT("BasketID: %s, ItemIDs: %s"), 
			//		*ItemPattern.BasketID, *ItemIDsStr);)
			//}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to read item data from the database."));
		}
		return DataList;
	}
};
