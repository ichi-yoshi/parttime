// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PartTimeCharacterMovement.generated.h"

/**
 * 
 */
UCLASS()
class PARTTIME_API UPartTimeCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	// めり込みを解消するときの移動量。商品が相手のときは自分を動かさない
	virtual FVector GetPenetrationAdjustment(const FHitResult& Hit) const override;

	// 歩ける面か。商品の上は歩ける面として扱わない
	virtual bool IsWalkable(const FHitResult& Hit) const override;
};
