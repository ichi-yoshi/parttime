#pragma once

#include "CoreMinimal.h"
#include "SQLiteRow.generated.h"

USTRUCT(BlueprintType)
struct PARTTIME_API FSQLiteRow
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "SQLite")
    TMap<FString, FString> Columns;

    FString GetString(const FString& ColumnName, const FString& DefaultValue = TEXT("")) const
    {
        const FString* Found = Columns.Find(ColumnName);
        return Found ? *Found : DefaultValue;
    }

    int32 GetInt(const FString& ColumnName, int32 DefaultValue = 0) const
    {
        const FString* Found = Columns.Find(ColumnName);
        return Found ? FCString::Atoi(**Found) : DefaultValue;
    }

    float GetFloat(const FString& ColumnName, float DefaultValue = 0.0f) const
    {
        const FString* Found = Columns.Find(ColumnName);
        return Found ? FCString::Atof(**Found) : DefaultValue;
    }
};