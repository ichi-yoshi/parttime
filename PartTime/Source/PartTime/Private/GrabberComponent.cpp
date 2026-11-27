// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

#include "PartTimeCollisionChannels.h"
#include "ItemActor.h"

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

	// 離した商品の無視の解除を、毎フレームチェック
	RestoreReleasedComponents();

	// 掴んでいるものがあれば、目の前に追従させる
	if (PhysicsHandle->GetGrabbedComponent() == nullptr) { return; }

	// カメラ位置と正面ベクトルから、目の前の座標を計算
	FVector CameraLocation = ViewCamera->GetComponentLocation();
	FVector CameraForward = ViewCamera->GetForwardVector();
	FVector TargetLocation = CameraLocation + CameraForward * HoldDistance;

	// カメラの向き * 相対の向きを求めると、視点を動かしてもものは手に持った向きのままついてくる
	// 回転を足すときは、相対の向きの左側から掛けるとカメラから見た軸で回る
	const FQuat TargetRotation = ViewCamera->GetComponentQuat() * HeldRelativeRotation;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, TargetRotation.Rotator());
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
		ECC_Grabbable,// 商品だけがBlockするチャンネル
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

	// ものの重心を掴む点とする。回転の軸になる
	const FVector GrabLocation = HitComponent->GetCenterOfMass();

	// カメラから掴む点までの距離を測る
	const FVector CameraLocation = ViewCamera->GetComponentLocation();
	HoldDistance = FMath::Clamp(FVector::Distance(CameraLocation, GrabLocation), MinHoldDistance, MaxHoldDistance);

	// 掴んだ時点のカメラから見たものの向きを保存
	HeldRelativeRotation = ViewCamera->GetComponentQuat().Inverse() * HitComponent->GetComponentQuat();

	// 指定した場所と角度でコンポーネントを掴む
	PhysicsHandle->GrabComponentAtLocationWithRotation(
		HitComponent,
		NAME_None,// どのボーンを掴むか(スタティックメッシュはNone)
		GrabLocation,
		HitComponent->GetComponentRotation()
	);

	// オーナーの移動処理が掴んだ商品を無視するようにする
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter->GetCapsuleComponent()->IgnoreComponentWhenMoving(HitComponent, true);

		// 解除待ちの商品をまた掴んだ場合は、待ちのリストから外す
		PendingRestoreComponents.Remove(HitComponent);
	}

	// 商品を掴まれている状態にする
	AItemActor* HeldItem = Cast<AItemActor>(HitComponent->GetOwner());
	if (HeldItem != nullptr) { HeldItem->SetHeld(true); }

	// 掴んだものが変わったことを通知
	OnHeldItemChanged.Broadcast(HeldItem);
}

void UGrabberComponent::Release()
{
	if (PhysicsHandle == nullptr || ViewCamera == nullptr) { return; }

	// ものを掴んでいるか
	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if (GrabbedComponent == nullptr) { return; }

	// 離す前に、商品を掴まれていない状態にする
	if (AItemActor* HeldItem = Cast<AItemActor>(GrabbedComponent->GetOwner()))
	{
		HeldItem->SetHeld(false);
	}

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		// 体にめり込んだまま解除すると押し出されるので、重なりがなくなるまで無視する
		// 解除はTickで行う
		PendingRestoreComponents.AddUnique(GrabbedComponent);
	}

	// ものを離す
	PhysicsHandle->ReleaseComponent();

	// nullを通知することで、掴んでいるものがなくなった
	OnHeldItemChanged.Broadcast(nullptr);
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

void UGrabberComponent::AddHoldRotation(const FVector2D& Delta)
{
	if (!IsGrabbing()) { return; }

	// カメラから見て左右の入力は上下の軸回り、上下の入力は左右の軸回りに回転させる
	const FQuat YawDelta(FVector::UpVector, FMath::DegreesToRadians(Delta.X * RotateSpeed));
	const FQuat PitchDelta(FVector::RightVector, FMath::DegreesToRadians(-Delta.Y * RotateSpeed));

	// 左から掛けるとカメラから見た軸で回る
	HeldRelativeRotation = YawDelta * PitchDelta * HeldRelativeRotation;
	HeldRelativeRotation.Normalize();// 誤差を防ぐために正規化
}

bool UGrabberComponent::IsGrabbing() const
{
	if (PhysicsHandle == nullptr || ViewCamera == nullptr) { return false; }

	// ものを掴んでいるか
	return PhysicsHandle->GetGrabbedComponent() != nullptr;
}

void UGrabberComponent::RestoreReleasedComponents()
{
	if (PendingRestoreComponents.IsEmpty()) { return; }

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr) { return; }

	UCapsuleComponent* Capsule = OwnerCharacter->GetCapsuleComponent();

	// 後ろから回して、途中で要素を消してもまだ見ていない要素の番号がずれないようにする
	for (int32 Index = PendingRestoreComponents.Num() - 1; Index >= 0; --Index)
	{
		UPrimitiveComponent* Component = PendingRestoreComponents[Index].Get();

		// 商品が消えていたら、リストから外す
		if (Component == nullptr)
		{
			PendingRestoreComponents.RemoveAtSwap(Index);
			continue;
		}

		// 今のカプセルの位置と向きで、カプセルの形が商品と重なっているかチェック
		const bool bOverlapping = Component->ComponentOverlapComponent(
			Capsule,
			Capsule->GetComponentLocation(),
			Capsule->GetComponentQuat(),
			FCollisionQueryParams()
		);

		// 重なりがなくなったら、移動処理が商品を無視するのをやめる
		if (!bOverlapping)
		{
			Capsule->IgnoreComponentWhenMoving(Component, false);
			PendingRestoreComponents.RemoveAtSwap(Index);
		}
	}
}