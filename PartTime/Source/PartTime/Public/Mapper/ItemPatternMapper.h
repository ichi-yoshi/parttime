#pragma once

#include "CoreMinimal.h"
#include "DataMapper.h"
#include "Model/ItemPatternModel.h"

class PARTTIME_API ItemPatternMapper : public IDataMapper<FItemPatternModel>
{
public:
	virtual FItemPatternModel MapRow(const FSQLiteRow& Row) const override 
	{
		// FItemPatternModelを初期化し、Rowからデータをマッピングする
		FItemPatternModel ItemPattern;
		ItemPattern.BasketID = Row.GetString(TEXT("Basket"));

		// 1から10までのアイテムIDを取得してItemIDsに追加
		for(int32 i = 1; i <= 10; ++i) 
		{
			FString ColumnName = FString::Printf(TEXT("ItemID%d"), i);
			int32 ItemID = Row.GetInt(ColumnName, 0);

			// 0でもItemIDsに追加することで、アイテムが配置されていないスロットを表現できる
			ItemPattern.ItemIDs.Add(ItemID);
		}
		return ItemPattern;
	}
};
