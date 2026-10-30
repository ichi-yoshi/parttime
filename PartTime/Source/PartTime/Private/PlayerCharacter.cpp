// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 掴んでいるものがあれば、目の前に追従させる
	if (PhysicsHandle->GetGrabbedComponent() == nullptr) { return; }

	// カメラ位置と正面ベクトルから、目の前の座標を計算
	FVector CameraLocation = FirstPersonCamera->GetComponentLocation();
	FVector CameraForward = FirstPersonCamera->GetForwardVector();
	FVector TargetLocation = CameraLocation + CameraForward * HoldDistance;

	// フィジックスハンドルのターゲット位置を更新
	PhysicsHandle->SetTargetLocation(TargetLocation);
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

bool APlayerCharacter::IsTargetInReach(FHitResult& OutHitResult) const
{
	// Line traceの始点と終点を、手のリーチ距離を考慮して計算
	FVector StartPoint = FirstPersonCamera->GetComonentLocation();
	FVector CameraForward = FirstPersonCamera->GetForwardVector();
	FVector EndPoint = StartPoint + (CameraForward * ReachDistance);

	// 自身のカプセルを無視するために、除外リストに自身を追加
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	return GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		StartPoint,
		EndPoint,
		ECC_Visiblity,// どのコリジョンチャンネルで判定するか
		TracePrams// 除外リスト
	);
}

void APlayerCharacter::Grab()
{
	// 目の前に掴めるものがあるかチェック
	FHitResult HitResult;
	if (!IsTargetInReach(HitResult)) { return; }

	// 当たったコンポーネントを取得
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	if (HitComponent == nullptr) { return; }

	// 掴めるものかチェック（物理シミュレーションされているか）
	if (!HitComponent->IsSimulatingPhysics()) { return; }

	// カメラとものの距離を、最小距離と最大距離の範囲内に収める
	FVector CameraLocation = FirstPersonCamera->GetComponentLocation();
	FVector ComponentLocation = HitComponent->GetComponentLocation();
	HoldDistance = FVector::Distance(CameraLocation, ComponentLocation);
	HoldDistance = FMath::Clamp(HoldDistance, MinHoldDistance, MaxHoldDistance);

	// 指定した場所と角度でコンポーネントを掴む
	PhysicsHandle->GrabComponentAtLocationWithRotation(
		HitComponent,
		NAME_None,// どのボーンを掴むか(スタティックメッシュはNone)
		ComponentLocation,
		HitComponent->GetComponentRotation()
	);
}

void APlayerCharacter::Release()
{
	// ものを掴んでいるか
	if (PhysicsHandle->GetGrabbedComponent() == nullptr) { return; }

	// ものを離す
	PhysicsHandle->ReleaseComponent();
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	// 左右の視点移動(Yaw)
	AddControllerYawInput(-LookAxis.X);

	// 上下の視点移動(Pitch)
	AddControllerPitchInput(LookAxis.Y);
}

void APlayerCharacter::Scroll(const FInputActionValue& Value)
{
	// ものを掴んでいるか
	if (PhysicsHandle->GetGrabbedComponent() == nullptr) { return; }

	// スクロール量を取得して、HoldDistanceを更新
	float ScrollValue = Value.Get<float>();
	HoldDistance += ScrollValue * Scrollspeed;

	HoldDistance = FMath::Clamp(HoldDistance, MinHoldDistance, MaxHoldDistance);
}

