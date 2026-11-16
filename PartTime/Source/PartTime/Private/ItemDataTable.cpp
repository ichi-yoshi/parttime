#include "ItemDataTable.h"
#include "SQLiteReader.h"
#include "Mapper/ItemDataMapper.h"
#include "Misc/Paths.h"

ItemDataTable::ItemDataTable()
{}

ItemDataTable::~ItemDataTable()
{}

TArray<FItemData> ItemDataTable::ReadItemDataTable()
{
	TArray<FItemData> ItemList;

	FString DbPath = FPaths::ProjectContentDir() / TEXT("DataBase/ItemData.db");

	FString Query = TEXT("SELECT * FROM Item");

	ItemDataMapper Mapper;

	bool bSuccess = FSQLiteGenericReader::ReadTable<FItemData>(DbPath, Query, Mapper, ItemList);

	if(bSuccess)
	{
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