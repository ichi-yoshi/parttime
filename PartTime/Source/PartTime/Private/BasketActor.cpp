#include "BasketActor.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

ABasketActor::ABasketActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BasketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BasketMesh"));
	SetRootComponent(BasketMesh);

	// 10個分の配置オフセット初期値
	for(int32 i = 0; i < 10; ++i)
	{
		float OffsetX = (i % 5 - 2) * 10.0f;
		float OffsetY = (i / 5 - 0.5f) * 15.0f;
		ItemSlotOffsets.Add(FVector(OffsetX, OffsetY, 5.0f));
	}
}

void ABasketActor::ClearBasket() 
{
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
	ClearBasket();

	UWorld* World = GetWorld();
	if(!World) { return; }

	for(int32 Index = 0; Index < ItemsToSpawn.Num(); ++Index)
	{
		const FItemData& Data = ItemsToSpawn[Index];

		// DBの ItemName ("apple" など) からBPパスを自動生成
		// 例: "/Game/Items/BP_Item_apple.BP_Item_apple_C"
		FString AssetName = FString::Printf(TEXT("BP_Item_%s"), *Data.ItemName);
		FString ClassPath = FString::Printf(TEXT("%s/%s.%s_C"), *ItemBlueprintFolder, *AssetName, *AssetName);

		// クラスを動的にロード
		UClass* LoadedClass = StaticLoadClass(AItemActor::StaticClass(), nullptr, *ClassPath);
		if(!LoadedClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("No class found: %s (ItemID: %d)"), *ClassPath, Data.ID);
			continue;
		}

		FVector LocalOffset = ItemSlotOffsets.IsValidIndex(Index)
			? ItemSlotOffsets[Index]
			: FVector(0.0f, 0.0f, 10.0f);

		FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, LocalOffset) * GetActorTransform();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		AItemActor* SpawnedItem = World->SpawnActor<AItemActor>(LoadedClass, SpawnTransform, SpawnParams);
		if(SpawnedItem)
		{
			SpawnedItem->ItemID = Data.ID;
			SpawnedItem->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			SpawnedItemActors.Add(SpawnedItem);
		}
	}
}