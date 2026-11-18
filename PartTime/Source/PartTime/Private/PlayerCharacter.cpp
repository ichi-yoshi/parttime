// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"

#include "GrabberComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// インスタンス生成
	// カメラ
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// ルートにアタッチ
	FirstPersonCamera->SetupAttachment(GetRootComponent());
	// カメラの位置を設定
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	// 視点回転を追従
	FirstPersonCamera->bUsePawnControlRotation = true;

	// フィジックスハンドル
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	Grabber = CreateDefaultSubobject<UGrabberComponent>(TEXT("Grabber"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// クロスヘアを生成して、画面に表示
	if (CrosshairWidgetClass)
	{
		// プレイヤーコントローラーを取得
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			// ウィジェットを生成して、画面に追加
			CrosshairWidget = CreateWidget<UUserWidget>(PlayerController, CrosshairWidgetClass);
			if (CrosshairWidget)
			{
				CrosshairWidget->AddToViewport();
			}
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// IMCを有効化
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// ローカルプレイヤーからサブシステムを取得して、IMCを追加
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	// Enhanced Input Componentにキャストして、入力アクションをバインド
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// グラブアクション
		if (GrabAction)
		{
			EnhancedInput->BindAction(GrabAction, ETriggerEvent::Started, this, &APlayerCharacter::Grab);
			EnhancedInput->BindAction(GrabAction, ETriggerEvent::Completed, this, &APlayerCharacter::Release);
		}

		// 視点移動
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}

		// スクロール
		if (ScrollAction)
		{
			EnhancedInput->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Scroll);
		}
	}
}

void APlayerCharacter::Grab()
{
	Grabber->Grab();
}

void APlayerCharacter::Release()
{
	Grabber->Release();
}

void APlayerCharacter::Scroll(const FInputActionValue& Value)
{
	Grabber->AdjustHoldDistance(Value.Get<float>());
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	// 左右の視点移動(Yaw)
	AddControllerYawInput(LookAxis.X);

	// 上下の視点移動(Pitch)
	AddControllerPitchInput(LookAxis.Y);
}

