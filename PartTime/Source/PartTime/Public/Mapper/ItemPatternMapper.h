#pragma once

#include "CoreMinimal.h"
#include "DataMapper.h"
#include "Model/ItemPatternModel.h"

class PARTTIME_API ItemPatternMapper : public IDataMapper<FItemPatternModel>
{
public:
	virtual FItemPatternModel MapRow(const FSQLiteRow& Row) const override 
	{
		FItemPatternModel ItemPattern;
		ItemPattern.BasketID = Row.GetString(TEXT("Basket"));

		for(int32 i = 1; i <= 10; ++i) 
		{
			FString ColumnName = FString::Printf(TEXT("ItemID%d"), i);
			int32 ItemID = Row.GetInt(ColumnName, 0);

			// 0の場合は無効なIDとして扱い、配列に追加しない
			if(ItemID > 0)
			{
				ItemPattern.ItemIDs.Add(ItemID);
			}
		}
		return ItemPattern;
	}
};
