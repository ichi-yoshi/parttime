#pragma once

#include "CoreMinimal.h"
#include "SQLiteRow.h"

template <typename T>
class IDataMapper
{
public:
    virtual ~IDataMapper() = default;

    // FSQLiteRow を型 T にマッピングする
    virtual T MapRow(const FSQLiteRow& Row) const = 0;
};