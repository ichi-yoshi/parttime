// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "PartTimePlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UUserWidget;

class UGrabberComponent;

/**
 * 
 */
UCLASS()
class PARTTIME_API APartTimePlayerController : public APlayerController
{
	GENERATED_BODY()

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


	// クロスヘアウィジェットクラス
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget>  CrosshairWidgetClass = nullptr;

	// クロスヘアUIインスタンス
	UPROPERTY()
	TObjectPtr<UUserWidget> CrosshairWidget = nullptr;


	// 視点調整用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float InitialPitch = -35.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float MinPitch = -70.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float MaxPitch = -10.0f;


	// 操作中PawnからGrabberを探す。見つからなければnullptr
	UGrabberComponent* GetGrabber() const;

	// 中継用関数
	void Look(const FInputActionValue& Value);
	void GrabStarted();
	void GrabCompleted();
	void Scroll(const FInputActionValue& Value);
};
