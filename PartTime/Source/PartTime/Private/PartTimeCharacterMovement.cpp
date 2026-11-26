// Fill out your copyright notice in the Description page of Project Settings.


#include "PartTimeCharacterMovement.h"
#include "ItemActor.h"

FVector UPartTimeCharacterMovement::GetPenetrationAdjustment(const FHitResult& Hit) const
{
	// 商品にめり込んだときは押し出されない。商品側が物理で押し戻される
	if (Cast<AItemActor>(Hit.GetActor()) != nullptr)
	{
		return FVector::ZeroVector;
	}

	// 他は元の処理のまま
	return Super::GetPenetrationAdjustment(Hit);
}

bool UPartTimeCharacterMovement::IsWalkable(const FHitResult& Hit) const
{
	// 商品の面は歩ける面として扱わない。横から押す相手になる
	if (Cast<AItemActor>(Hit.GetActor()) != nullptr)
	{
		return false;
	}

	// 他は元の処理のまま
	return Super::IsWalkable(Hit);
}

