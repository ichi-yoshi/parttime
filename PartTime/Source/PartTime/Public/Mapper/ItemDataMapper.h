#pragma once

#include "CoreMinimal.h"
#include "DataMapper.h"
#include "Model/ItemDataModel.h"

class PARTTIME_API ItemDataMapper : public IDataMapper<FItemData>
{
public:
	virtual FItemData MapRow(const FSQLiteRow& Row) const override 
	{
		FItemData Item;
		Item.ID = Row.GetInt(TEXT("ID"));
		Item.ItemName = Row.GetString(TEXT("Item"));
		Item.weight = Row.GetFloat(TEXT("Weight"));
		Item.Durability = Row.GetFloat(TEXT("Durability"));
		return Item;
	}
};
