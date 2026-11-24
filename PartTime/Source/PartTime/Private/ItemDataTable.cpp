#include "ItemDataTable.h"
#include "Mapper/ItemDataMapper.h"
#include "Mapper/ItemPatternMapper.h"
#include "Misc/Paths.h"

// データベースからアイテムデータを読み込む静的関数
TArray<FItemData> ItemDataTable::ReadItemDataTable()
{
	return LoadTableData<FItemData, ItemDataMapper>(TEXT("Item"));
}

TArray<FItemPatternModel> ItemDataTable::ReadItemPatternTable() 
{
	return LoadTableData<FItemPatternModel, ItemPatternMapper>(TEXT("ItemPattern"));
}