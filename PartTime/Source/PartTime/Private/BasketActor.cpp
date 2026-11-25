#include "BasketActor.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

ABasketActor::ABasketActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// バスケットメッシュを作成し、ルートコンポーネントとして設定する
	BasketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BasketMesh"));
	SetRootComponent(BasketMesh);

	// 初期配置のオフセットを設定する
	ItemSlots.Reserve(10);
	for(int32 i = 0; i < 10; ++i)
	{
		// スロットコンポーネントを作成し、バスケットメッシュにアタッチする
		FName SlotName = *FString::Printf(TEXT("Slot_%d"), i + 1);
		USceneComponent* SlotComp = CreateDefaultSubobject<USceneComponent>(SlotName);
		SlotComp->SetupAttachment(BasketMesh);

		// 初期配置（2列×5行）
		float OffsetX = (i % 5 - 2) * 10.0f;
		float OffsetY = (i / 5 - 0.5f) * 15.0f;
		SlotComp->SetRelativeLocation(FVector(OffsetX, OffsetY, 5.0f));

		// スロットコンポーネントを配列に追加
		ItemSlots.Add(SlotComp);
	}
}

void ABasketActor::ClearBasket() 
{
	// 既存のアイテムをすべて削除する
	for(AItemActor* ItemActor : SpawnedItemActors)
	{
		if(ItemActor)
		{
			ItemActor->Destroy();
		}
	}
	SpawnedItemActors.Empty();
}

void ABasketActor::SpawnItemsInBasket(const TArray<FItemData>& ItemsToSpawn) 
{
	// まず既存のアイテムをクリアする
	ClearBasket();

	UWorld* World = GetWorld();
	if(!World) { return; }

	for(int32 Index = 0; Index < ItemsToSpawn.Num(); ++Index)
	{
		// 配列の範囲外をチェック
		const FItemData& Data = ItemsToSpawn[Index];

		if(Data.ID <= 0) 
		{
			continue; // 無効なIDの場合はスキップ
		}

		// DBの ItemName ("apple" など) からBPパスを自動生成
		// 例: "/Game/Asset/Syouhin/BP_Item_apple.BP_Item_apple_C"
		FString AssetName = FString::Printf(TEXT("BP_Item_%s"), *Data.ItemName);
		FString ClassPath = FString::Printf(TEXT("%s/%s.%s_C"), *ItemBlueprintFolder, *AssetName, *AssetName);

		// クラスを動的にロード
		UClass* LoadedClass = StaticLoadClass(AItemActor::StaticClass(), nullptr, *ClassPath);
		if(!LoadedClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("No class found: %s (ItemID: %d)"), *ClassPath, Data.ID);
			continue;
		}

		// スロットの位置を基準にスポーンする
		FTransform SpawnTransform;
		if(ItemSlots.IsValidIndex(Index) && ItemSlots[Index])
		{
			// スロットの位置を基準にスポーンする
			SpawnTransform = ItemSlots[Index]->GetComponentTransform();
		}
		else
		{
			// スロットが存在しない場合はバスケットの位置を基準にスポーンする
			SpawnTransform = GetActorTransform();
		}

		// スポーンパラメータを設定
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		// アイテムをスポーン
		AItemActor* SpawnedItem = World->SpawnActor<AItemActor>(LoadedClass, SpawnTransform, SpawnParams);
		if(SpawnedItem)
		{
			SpawnedItem->ItemID = Data.ID;		// アイテムIDを設定
			SpawnedItem->ApplyItemData(Data);	// アイテムデータを適用
			SpawnedItem->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);	// バスケットにアタッチ
			SpawnedItemActors.Add(SpawnedItem);	// スポーンしたアイテムをリストに追加
		}
	}
}