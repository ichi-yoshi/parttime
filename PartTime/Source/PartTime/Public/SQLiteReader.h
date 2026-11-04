#pragma once

#include "CoreMinimal.h"
#include "SQLiteRow.h"
#include "DataMapper.h"
#include "SQLiteDatabase.h" // UE5組み込みのSQLiteモジュールヘッダー

class PARTTIME_API FSQLiteGenericReader
{
public:
	// 汎用読み取り関数：指定されたテーブルからすべての行を取得する
	static bool ReadanyTable(const FString& DbPath, const FString& Query, TArray<FSQLiteRow>& OutRows)
	{
		FString Query = FString::Printf(TEXT("SELECT * FROM %s"), *TableName);
		return ExecuteQuery(DbPath, Query, OutRows);
	}

	static bool GetALLTableNames(const FString& DbPath, TArray<FString>& OutTableNames)
	{
	    TArray<FSQLiteRow> Rows;
		FStringQuery = TEXT("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';");

        if(!ExecuteQuery(DbPath, Query, Rows)) 
        {
            return false;
        }
        for(const FSQLiteRow& Row : Rows) 
        {
			OutTableNames.Add(Row.GetString(TEXT("name")));
        }
		return true;
	}

private:
    // 汎用読み取り関数：SQLを実行し、FSQLiteRowの配列を返す
    static bool ExecuteQuery(const FString& DbPath, const FString& Query, TArray<FSQLiteRow>& OutRows)
    {
        FSQLiteDatabase Database;
        if(!Database.Open(*DbPath, ESQLiteDatabaseOpenMode::ReadOnly))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to open SQLite database: %s"), *DbPath);
            return false;
        }

        FSQLiteResultSet ResultSet;
        if(!Database.Execute(*Query, ResultSet))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to execute query: %s"), *Query);
            Database.Close();
            return false;
        }

        for(const FSQLiteRowId& RowId : ResultSet)
        {
            FSQLiteRow Row;
            for(const FSQLiteColumn& Column : ResultSet.GetColumns())
            {
                FString Value;
                ResultSet.GetString(*Column.GetName(), Value);
                Row.Columns.Add(Column.GetName(), Value);
            }
            OutRows.Add(Row);
        }

        Database.Close();
        return true;
    }

    // テンプレート関数：データマッパーを使用して指定型の配列として読み取る
    template <typename T>
    static bool ReadTable(const FString& DbPath, const FString& Query, const IDataMapper<T>& Mapper, TArray<T>& OutResults)
    {
        TArray<FSQLiteRow> RawRows;
        if(!ExecuteQuery(DbPath, Query, RawRows))
        {
            return false;
        }

        OutResults.Reserve(RawRows.Num());
        for(const FSQLiteRow& Row : RawRows)
        {
            OutResults.Add(Mapper.MapRow(Row));
        }

        return true;
    }
};