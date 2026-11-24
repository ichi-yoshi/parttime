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

void ABasketActor::SpawnItemsInBasket(const TArray<int32>& ItemIDs) 
{
	ClearBasket();

	UWorld* World = GetWorld();
	if(!World) { return; }

	for(int32 Index = 0; Index < ItemIDs.Num(); ++Index)
	{
		int32 TargetItemID = ItemIDs[Index];

		TSubclassOf<AItemActor>* ActorClassPtr = ItemActorClassMap.Find(TargetItemID);
		if(!ActorClassPtr || !(*ActorClassPtr))
		{
			UE_LOG(LogTemp, Warning, TEXT("Basket '%s': ItemID %d No Class"), *BasketID, TargetItemID);
			continue;
		}

		FVector LocalOffset = ItemSlotOffsets.IsValidIndex(Index)
			? ItemSlotOffsets[Index]
			: FVector(0.0f, 0.0f, 10.0f);

		FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, LocalOffset) * GetActorTransform();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		AItemActor* SpawnedItem = World->SpawnActor<AItemActor>(*ActorClassPtr, SpawnTransform, SpawnParams);
		if(SpawnedItem)
		{
			// スポーンした AItemActor に ItemID をセットしておく（後の GameMode ループで ApplyItemData される）
			SpawnedItem->ItemID = TargetItemID;
			SpawnedItem->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			SpawnedItemActors.Add(SpawnedItem);
		}
	}
}