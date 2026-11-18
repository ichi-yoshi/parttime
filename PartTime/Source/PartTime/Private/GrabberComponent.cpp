// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	// フィジックスハンドルとカメラを保持しておく
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	ViewCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle == nullptr || ViewCamera == nullptr) { return; }

	// 掴んでいるものがあれば、目の前に追従させる
	if (PhysicsHandle->GetGrabbedComponent() == nullptr) { return; }

	// カメラ位置と正面ベクトルから、目の前の座標を計算
	FVector CameraLocation = ViewCamera->GetComponentLocation();
	FVector CameraForward = ViewCamera->GetForwardVector();
	FVector TargetLocation = CameraLocation + CameraForward * HoldDistance;

	// フィジックスハンドルのターゲット位置を更新
	PhysicsHandle->SetTargetLocation(TargetLocation);
}



bool UGrabberComponent::IsTargetInReach(FHitResult& OutHitResult) const
{
	if (PhysicsHandle == nullptr || ViewCamera == nullptr) { return false; }

	// Line traceの始点と終点を、手のリーチ距離を考慮して計算
	FVector StartPoint = ViewCamera->GetComponentLocation();
	FVector CameraForward = ViewCamera->GetForwardVector();
	FVector EndPoint = StartPoint + (CameraForward * ReachDistance);

	// 自身のカプセルを無視するために、除外リストに自身を追加
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		StartPoint,
		EndPoint,
		ECC_Visibility,// どのコリジョンチャンネルで判定するか
		TraceParams// 除外リスト
	);
}

void UGrabberComponent::Grab()
{
	if (PhysicsHandle == nullptr || ViewCamera == nullptr) { return; }

	// 目の前に掴めるものがあるかチェック
	FHitResult HitResult;
	if (!IsTargetInReach(HitResult)) { return; }

	// 当たったコンポーネントを取得
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	if (HitComponent == nullptr) { return; }

	// 掴めるものかチェック（物理シミュレーションされているか）
	if (!HitComponent->IsSimulatingPhysics()) { return; }

	// カメラとものの距離を、最小距離と最大距離の範囲内に収める
	FVector CameraLocation = ViewCamera->GetComponentLocation();
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

void UGrabberComponent::Release()
{
	if (PhysicsHandle == nullptr || ViewCamera == nullptr) { return; }

	// ものを掴んでいるか
	if (PhysicsHandle->GetGrabbedComponent() == nullptr) { return; }

	// ものを離す
	PhysicsHandle->ReleaseComponent();
}

void UGrabberComponent::AdjustHoldDistance(float Delta)
{
	if (PhysicsHandle == nullptr || ViewCamera == nullptr) { return; }

	// ものを掴んでいるか
	if (PhysicsHandle->GetGrabbedComponent() == nullptr) { return; }

	// スクロール量を取得して、HoldDistanceを更新
	HoldDistance += Delta * ScrollSpeed;

	HoldDistance = FMath::Clamp(HoldDistance, MinHoldDistance, MaxHoldDistance);
}

bool UGrabberComponent::IsGrabbing() const
{
	if (PhysicsHandle == nullptr || ViewCamera == nullptr) { return false; }

	// ものを掴んでいるか
	return PhysicsHandle->GetGrabbedComponent() != nullptr;
}