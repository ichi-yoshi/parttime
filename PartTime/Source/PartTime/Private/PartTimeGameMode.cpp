// Fill out your copyright notice in the Description page of Project Settings.


#include "PartTimeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ItemDataTable.h"
#include "ItemActor.h"

void PartTimeGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<FItemData> ItemDataList = ItemDataTable::ReadItemDataTable();

	TMap<int32, FItemData> ItemDataMap;
	for (const FItemData& ItemData : ItemDataList)
	{
		ItemDataMap.Add(ItemData.ID, ItemData);
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemActor::StaticClass(), FoundActors);

	for(AActor* Actor : FoundActors)
	{
		AItemActor* ItemActor = Cast<AItemActor>(Actor);
		if(ItemActor)
		{
			int32 ItemID = ItemActor->GetItemID();
			if(ItemDataMap.Contains(ItemID))
			{
				const FItemData& ItemData = ItemDataMap[ItemID];
				ItemActor->SetItemData(ItemData);
			}
		}
	}
}
