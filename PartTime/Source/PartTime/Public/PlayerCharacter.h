// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UPhysicsHandleComponent;
class UInputAction;
class UInputMappingContext;
class UUserWidget;

UCLASS()
class PARTTIME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	// 手のリーチ範囲内のターゲットがあるか判定
	// @param OutHitResult:ヒットした部品・座標などのデータ構造体(参照渡し)
	// @return:ヒットしたらtrue
	bool IsTargetInReach(FHitResult& OutHitResult) const;

	// マウス操作処理
	// 掴む
	void Grab();

	// 離す
	void Release();

	// 視点移動
	void Look(const FInputActionValue& Value);

	// スクロール(奥行き)
	void Scroll(const FInputActionValue& Value);


	// 一人称カメラ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera = nullptr;


	// 物理グラブフィジックスハンドル
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle = nullptr;

	// 手のリーチ距離、もの近づける最小距離、遠ざける最大距離、スクロール速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	float ReachDistance = 200.0f, MinHoldDistance = 10.0f, MaxHoldDistance = 500.0f, Scrollspeed = 50.0f;

	// 現在ものを掴んでいる距離
	float HoldDistance = 0.0f;


	// インプットアクション
	// グラブ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> GrabAction = nullptr;

	// 視点移動
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction = nullptr;

	// スクロール
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ScrollAction = nullptr;


	// IMCの設定
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;


	// クロスヘアウィジェットクラス
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget>  CrosshairWidgetClass = nullptr;

	// クロスヘアUIインスタンス
	UPROPERTY()
	TObjectPtr<UUserWidget> CrosshairWidget = nullptr;
};
