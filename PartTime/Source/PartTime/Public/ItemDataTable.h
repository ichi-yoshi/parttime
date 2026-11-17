#pragma once
#include "CoreMinimal.h"
#include "Model/ItemDataModel.h"

class PARTTIME_API ItemDataTable
{
public:
	ItemDataTable();
	~ItemDataTable();

	// データベースからアイテムデータを読み込む静的関数
	static TArray<FItemData> ReadItemDataTable();
};
