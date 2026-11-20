// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "PartTimePlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UUserWidget;
class UItemInfoWidget;

class UGrabberComponent;
class AItemActor;

/**
 * 
 */
UCLASS()
class PARTTIME_API APartTimePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// 掴んだものが変わったことを受け取る。nullで掴んでいない
	UFUNCTION()
	void HandleHeldItemChanged(AItemActor* HeldItem);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	// IMCの設定
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;
	

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

	// 回転
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RotateAction = nullptr;

	// ウィジェットクラス
	// クロスヘア
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget>  CrosshairWidgetClass = nullptr;
	// クロスヘアUIインスタンス
	UPROPERTY()
	TObjectPtr<UUserWidget> CrosshairWidget = nullptr;

	// 商品情報
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemInfoWidget> ItemInfoWidgetClass = nullptr;
	// 商品情報UIインスタンス
	UPROPERTY()
	TObjectPtr<UItemInfoWidget> ItemInfoWidget = nullptr;


	// 視点調整用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float InitialPitch = -35.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float MinPitch = -70.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float MaxPitch = -10.0f;


	// 回転用
	bool bRotateHeld = false;// 右クリック押しているか


	// 操作中PawnからGrabberを探す。見つからなければnullptr
	UGrabberComponent* GetGrabber() const;

	// 中継用関数
	void Look(const FInputActionValue& Value);
	void GrabStarted();
	void GrabCompleted();
	void Scroll(const FInputActionValue& Value);
	void RotateStarted();
	void RotateCompleted();
};
