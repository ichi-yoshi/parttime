// Fill out your copyright notice in the Description page of Project Settings.


#include "PartTimePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "GrabberComponent.h"
#include "GameFramework/Pawn.h"
#include "ItemInfoWidget.h"
#include "ItemActor.h"

void APartTimePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		// IMCを有効化
		// ローカルプレイヤーからサブシステムを取得して、IMCを追加
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}

		// クロスヘアを生成して、画面に表示
		if (CrosshairWidgetClass)
		{
			// ウィジェットを生成して、画面に追加
			CrosshairWidget = CreateWidget<UUserWidget>(this, CrosshairWidgetClass);
			if (CrosshairWidget)
			{
				CrosshairWidget->AddToViewport();
			}
		}

		// 商品情報ウィジェットを生成して、画面に表示
		if (ItemInfoWidgetClass)
		{
			// ウィジェットを生成して、画面に追加
			ItemInfoWidget = CreateWidget<UItemInfoWidget>(this, ItemInfoWidgetClass);
			if (ItemInfoWidget)
			{
				ItemInfoWidget->AddToViewport();
			}
		}	
	}
}

void APartTimePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enhanced Input Componentにキャストして、入力アクションをバインド
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// グラブアクション
		if (GrabAction)
		{
			EnhancedInput->BindAction(GrabAction, ETriggerEvent::Started, this, &APartTimePlayerController::GrabStarted);
			EnhancedInput->BindAction(GrabAction, ETriggerEvent::Completed, this, &APartTimePlayerController::GrabCompleted);
		}

		// 視点移動
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APartTimePlayerController::Look);
		}

		// スクロール
		if (ScrollAction)
		{
			EnhancedInput->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &APartTimePlayerController::Scroll);
		}

		// 回転
		if (RotateAction)
		{
			EnhancedInput->BindAction(RotateAction, ETriggerEvent::Started, this, &APartTimePlayerController::RotateStarted);
			EnhancedInput->BindAction(RotateAction, ETriggerEvent::Completed, this, &APartTimePlayerController::RotateCompleted);
		}

		// 移動
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APartTimePlayerController::Move);
		}
	}
}

void APartTimePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn == nullptr) { return; }

	// Pawnを所持したときに、見下ろしの角度にする
	SetControlRotation(FRotator(InitialPitch, InPawn->GetActorRotation().Yaw, 0.0f));
	
	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = MinPitch;
		PlayerCameraManager->ViewPitchMax = MaxPitch;
	}

	// Grabberの掴んだものが変わった通知を受け取るように登録
	if (UGrabberComponent* Grabber = InPawn->FindComponentByClass<UGrabberComponent>())
	{
		Grabber->OnHeldItemChanged.AddDynamic(this, &APartTimePlayerController::HandleHeldItemChanged);
	}
}

UGrabberComponent* APartTimePlayerController::GetGrabber() const
{
	APawn* ControllerPawn = GetPawn();
	return ControllerPawn ? ControllerPawn->FindComponentByClass<UGrabberComponent>() : nullptr;
}

void APartTimePlayerController::GrabStarted()
{
	if (UGrabberComponent* Grabber = GetGrabber()) { Grabber->Grab(); }
}

void APartTimePlayerController::GrabCompleted()
{
	if (UGrabberComponent* Grabber = GetGrabber()) { Grabber->Release(); }
}

void APartTimePlayerController::Scroll(const FInputActionValue& Value)
{
	if (UGrabberComponent* Grabber = GetGrabber()) { Grabber->AdjustHoldDistance(Value.Get<float>()); }
}

void APartTimePlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	// 右クリック中で、ものを掴んでいるときは視点ではなくものを回転させる
	UGrabberComponent* Grabber = GetGrabber();
	if (bRotateHeld && Grabber && Grabber->IsGrabbing())
	{
		Grabber->AddHoldRotation(LookAxis);
		return;
	}

	AddYawInput(LookAxis.X);
	AddPitchInput(LookAxis.Y);
}

void APartTimePlayerController::RotateStarted()
{
	bRotateHeld = true;
}

void APartTimePlayerController::RotateCompleted()
{
	bRotateHeld = false;
}

void APartTimePlayerController::HandleHeldItemChanged(AItemActor* HeldItem)
{
	if (ItemInfoWidget == nullptr) { return; }

	if (HeldItem)
	{
		// 商品を掴んだらその商品のデータを渡して表示
		ItemInfoWidget->ShowItemInfo(HeldItem->ItemData);
	}
	else
	{
		// 商品を離したら非表示
		ItemInfoWidget->HideItemInfo();
	}
}

void APartTimePlayerController::Move(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn == nullptr) { return; }

	// X:左右、Y:前後
	const FVector2D MoveAxis = Value.Get<FVector2D>();

	// 視点の左右の向き（Yaw）を使って、見下ろしでも地面と平行に移動するようにする
	const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	const FVector Forward = YawRotation.RotateVector(FVector::ForwardVector);
	const FVector Right = YawRotation.RotateVector(FVector::RightVector);

	// 移動方向を渡すと、CharacterMovementが速度を計算して動かす
	ControlledPawn->AddMovementInput(Forward, MoveAxis.Y);
	ControlledPawn->AddMovementInput(Right, MoveAxis.X);
}