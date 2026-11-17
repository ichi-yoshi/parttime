#pragma once

#include "CoreMinimal.h"
#include "SQLiteRow.h"
#include "DataMapper.h"
#include "SQLiteDatabase.h"

class PARTTIME_API FSQLiteGenericReader
{
public:
	// データベース内の指定したテーブルから全レコードを取得する
	static bool ReadAnyTable(const FString& DbPath, const FString& TableName, TArray<FSQLiteRow>& OutRows)
	{
		// SQL文を組み立て、クエリに渡して結果を格納する
		FString Query = FString::Printf(TEXT("SELECT * FROM %s"), *TableName);
		return ExecuteQuery(DbPath, Query, OutRows);
	}

	// データベース内の全テーブル名を取得する
	static bool GetAllTableNames(const FString& DbPath, TArray<FString>& OutTableNames)
	{
		TArray<FSQLiteRow> Rows;
		FString Query = TEXT("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';"); // SQLiteの内部テーブルを除外する

		// ExecuteQuery関数を呼び出して、クエリを実行し、結果をRowsに格納する
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

	// データベース内の指定したテーブルから全レコードを取得し、指定したデータマッパーを使用して結果を変換する
	template <typename T>
	static bool ReadTable(const FString& DbPath, const FString& Query, const IDataMapper<T>& Mapper, TArray<T>& OutResults)
	{
		// 生データの取得
		TArray<FSQLiteRow> RawRows;
		if(!ExecuteQuery(DbPath, Query, RawRows))
		{
			return false;
		}

		OutResults.Reserve(RawRows.Num());	// 事前に必要な容量を確保しておくことで、メモリ再割り当ての回数を減らす
		for(const FSQLiteRow& Row : RawRows)
		{
			OutResults.Add(Mapper.MapRow(Row));
		}

		return true;
	}

private:
	// データベース接続とクエリ実行を行い、結果をOutRowsに格納する
	static bool ExecuteQuery(const FString& DbPath, const FString& Query, TArray<FSQLiteRow>& OutRows)
	{
		// データベースを開く
		FSQLiteDatabase Database;
		if(!Database.Open(*DbPath, ESQLiteDatabaseOpenMode::ReadOnly))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to open SQLite database: %s"), *DbPath);
			return false;
		}

		// クエリを実行し、結果をOutRowsに格納する
		int64 Result = Database.Execute(*Query, [&OutRows](const FSQLitePreparedStatement& Statement)
			{
				// 新しいFSQLiteRowを作成し、各列の値を取得してRowに追加する
				FSQLiteRow Row;
				TArray<FString> ColumnNames = Statement.GetColumnNames();

				// 各列の値を取得してRowに追加する
				for(const FString& ColName : ColumnNames)
				{
					FString Value;
					Statement.GetColumnValueByName(*ColName, Value);
					Row.Columns.Add(ColName, Value);
				}

				// RowをOutRowsに追加する
				OutRows.Add(Row);
				return ESQLitePreparedStatementExecuteRowResult::Continue;
			});

		// クエリの実行結果を確認する
		bool bSuccess=(Result >= 0);

		Database.Close();
		return bSuccess;
	}
};