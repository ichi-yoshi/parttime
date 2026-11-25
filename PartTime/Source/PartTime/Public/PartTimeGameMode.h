#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Model/ItemDataModel.h"
#include "Model/ItemPatternModel.h"
#include "PartTimeGameMode.generated.h"

UCLASS()
class PARTTIME_API APartTimeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// ゲーム開始時に呼び出される関数
	virtual void BeginPlay() override;

	// データベースからアイテムデータをロードする関数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Database")
	TMap<int32, FItemData> ItemDataMap; // アイテムIDをキー、FItemDataを値とするマップ
	TMap<FString, TArray<int32>> ItemPatternDataMap; // バスケットIDをキー、FItemPatternDataを値とするマップ
};
