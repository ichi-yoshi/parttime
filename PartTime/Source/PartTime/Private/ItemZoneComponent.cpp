// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemZoneComponent.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "Engine/CollisionProfile.h"
#include "ItemActor.h"
#include "PartTimeCollisionChannels.h"

UItemZoneComponent::UItemZoneComponent()
{
	// 範囲自体はものとぶつからない。自身の形を渡してもらうためにコリジョンは無効化する
	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SetGenerateOverlapEvents(false);
}

TArray<AItemActor*> UItemZoneComponent::GetItemsInZone() const
{
	TArray<AItemActor*> Result;

	UWorld* World = GetWorld();
	if(World == nullptr) { return Result; }

	// 1.範囲ボックスと重なっている商品を候補として収集
	// 商品の種類だけを対象とする
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_ItemObject);

	// カゴ自身は対象外
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ItemZone), false);
	QueryParams.AddIgnoredActor(GetOwner());

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByObjectType(
		Overlaps,
		GetComponentLocation(),// ボックスの中心
		GetComponentQuat(),// ボックスの回転
		ObjectParams,
		FCollisionShape::MakeBox(GetScaledBoxExtent()),// ボックスの大きさ
		QueryParams
	);

	// 2.候補の中から、商品の中心点が範囲内にあるものだけを残す
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AItemActor* Item = Cast<AItemActor>(Overlap.GetActor());
		if (Item == nullptr || Item->GetRootComponent() == nullptr) { continue; }

		// 掴まれている商品は数えない
		if (Item->IsHeld()) { continue; }

		// 商品の中心 = メッシュのバウンティングボックスの中心
		const FVector ItemCenter = Item->GetRootComponent()->Bounds.Origin;
		if (!IsPointInZone(ItemCenter)) { continue; }

		// 同じ商品が重複しないように
		Result.AddUnique(Item);
	}

	return Result;
}

bool UItemZoneComponent::IsPointInZone(const FVector& WorldPoint) const
{
	// 点をボックスから見た座標に変換する。ボックスが回転していても正しく判定できるため
	const FVector LocalPoint = GetComponentTransform().InverseTransformPosition(WorldPoint);

	// 逆変換でスケールを戻して、スケールを含まない大きさと比べる
	const FVector Extent = GetUnscaledBoxExtent();
	return FBox(-Extent, Extent).IsInsideOrOn(LocalPoint);
}