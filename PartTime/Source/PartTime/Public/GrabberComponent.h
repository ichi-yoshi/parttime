// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "GrabberComponent.generated.h"

class UPhysicsHandleComponent;
class UPrimitiveComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PARTTIME_API UGrabberComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
private:
	// 手のリーチ範囲内のターゲットがあるか判定
	// @param OutHitResult:ヒットした部品・座標などのデータ構造体(参照渡し)
	// @return:ヒットしたらtrue
	bool IsTargetInReach(FHitResult& OutHitResult) const;

	// マウス操作処理
	void Grab();// 掴む
	void Release();// 離す
	//void Look(const FInputActionValue& Value);// 視点移動
	void Scroll(const FInputActionValue& Value);// スクロール(奥行き)



	UPROPERTY()
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle = nullptr;

	// 手のリーチ距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	float ReachDistance = 200.0f;
	// 掴むものの最小距離・最大距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	float MinHoldDistance = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	float MaxHoldDistance = 500.0f;
	// スクロール速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	float ScrollSpeed = 50.0f;

	// 現在ものを掴んでいる距離
	float HoldDistance = 0.0f;
};
