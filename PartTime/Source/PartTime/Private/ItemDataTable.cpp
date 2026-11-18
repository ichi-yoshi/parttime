#include "ItemDataTable.h"
#include "SQLiteReader.h"
#include "Mapper/ItemDataMapper.h"
#include "Misc/Paths.h"

ItemDataTable::ItemDataTable()
{}

ItemDataTable::~ItemDataTable()
{}

// データベースからアイテムデータを読み込む静的関数
TArray<FItemData> ItemDataTable::ReadItemDataTable()
{
	// アイテムデータを格納する配列を初期化
	TArray<FItemData> ItemList;

	// データベースのパスを指定
	FString DbPath = FPaths::ProjectContentDir() / TEXT("DataBase/ItemData.db");

	// SQLクエリを作成して、Itemテーブルから全てのデータを取得
	FString Query = TEXT("SELECT * FROM Item");

	// ItemDataMapperを使用して、SQLiteRowをFItemDataにマッピングする
	ItemDataMapper Mapper;

	// データベースからアイテムデータを読み込み、ItemListに格納
	bool bSuccess = FSQLiteGenericReader::ReadTable<FItemData>(DbPath, Query, Mapper, ItemList);

	// デバッグ用に読み込んだアイテムデータをログに出力
	if(bSuccess)
	{
		// 読み込んだアイテムデータをログに出力
		for(const FItemData& Item : ItemList)
		{
			UE_LOG(LogTemp, Log, TEXT("ID: %d, Name: %s, Weight: %f,Durability: %f"),
				Item.ID, *Item.ItemName, Item.weight, Item.Durability);
		}
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to read item data from the database."));
	}
	return ItemList;
}