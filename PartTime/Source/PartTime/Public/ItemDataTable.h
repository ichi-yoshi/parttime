#pragma once
#include "CoreMinimal.h"
#include "Model/ItemDataModel.h"

class PARTTIME_API ItemDataTable
{
public:
	ItemDataTable();
	~ItemDataTable();

	static TArray<FItemData> ReadItemDataTable();
};
