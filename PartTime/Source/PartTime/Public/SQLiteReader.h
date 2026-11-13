#pragma once

#include "CoreMinimal.h"
#include "SQLiteRow.h"
#include "DataMapper.h"
#include "SQLiteDatabase.h"

class PARTTIME_API FSQLiteGenericReader
{
public:
	static bool ReadanyTable(const FString& DbPath, const FString& TableName, TArray<FSQLiteRow>& OutRows)
	{
		FString Query = FString::Printf(TEXT("SELECT * FROM %s"), *TableName);
		return ExecuteQuery(DbPath, Query, OutRows);
	}

	static bool GetALLTableNames(const FString& DbPath, TArray<FString>& OutTableNames)
	{
		TArray<FSQLiteRow> Rows;
		FString Query = TEXT("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';");

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

private:
	static bool ExecuteQuery(const FString& DbPath, const FString& Query, TArray<FSQLiteRow>& OutRows)
	{
		FSQLiteDatabase Database;
		if(!Database.Open(*DbPath, ESQLiteDatabaseOpenMode::ReadOnly))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to open SQLite database: %s"), *DbPath);
			return false;
		}

	/*	int64 Result = Database.Execute(*Query, [&OutRows](const FSQLitePreparedStatement& Statement)
			{
				FSQLiteRow Row;
				TArray<FString> ColumnNames = Statement.GetColumnNames();
				for(const FString& ColName : ColumnNames)
				{
					FString Value;
					Statement.GetColumnValueByName(*ColName, Value);
					Row.Columns.Add(ColName, Value);
				}
				OutRows.Add(Row);
				return ESQLitePreparedStatementExecuteRowResult::Continue;
			});
		bool bSuccess=(Result >= 0);

		Database.Close();
		return bSuccess;*/
		FSQLitePreparedStatement Statement;
		if(!Statement.Create(Database, *Query, ESQLitePreparedStatementFlags::Persistent))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create statement: %s"), *Query);
			Database.Close();
			return false;
		}

		// Step() で1行ずつ確実に進め、終端に達したら必ずループを抜ける
		while(Statement.Step() == ESQLiteStatementStepResult::Row)
		{
			FSQLiteRow Row;
			const TArray<FString>& ColumnNames = Statement.GetColumnNames();
			for(const FString& ColName : ColumnNames)
			{
				FString Value;
				Statement.GetColumnValueByName(*ColName, Value);
				Row.Columns.Add(ColName, Value);
			}
			OutRows.Add(Row);
		}

		Statement.Destroy();
		Database.Close();
		return true;
	}
};