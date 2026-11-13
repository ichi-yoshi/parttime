// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

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

	// ...
	
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



bool UGrabberComponent::IsTargetInReach(FHitResult& OutHitResult) const
{
	AActor* Owner = GetOwner();
	if (!Owner) { return false; }

	// 親アクタからプレイヤーコントローラーを取得
	APawn* OwnerPawn = Cast<APawn>(Owner);
	APlayerController* PC = OwnerPawn ? Cast<APlayerController>(OwnerPawn->GetController()) : nullptr;
	if (!PC) { return false; }

	// マウスカーソルの真下にあるオブジェクトを判定
	if (!PC->GetHitResultUnderCursor(ECC_Visibility, false, OutHitResult)) { return false; }

	// ヒットしたオブジェクトまでの距離が手のリーチ距離以内か判定
	float Distance = FVector::Distance(Owner->GetActorLocation(), OutHitResult.ImpactPoint);
	return Distance <= ReachDistance;
}

void UGrabberComponent::Grab()
{
	if (!PhysicsHandle) { return; }

	// カーソル下に掴めるものがあるかチェック
	FHitResult HitResult;
	if (!IsTargetInReach(HitResult)) { return; }

	// 当たったコンポーネントが物理シミュレーション有効かチェック
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	if (!HitComponent || !HitComponent->IsSimulatingPhysics()) { return; }

	// 掴む処理
	FVector ComponentLocation = HitComponent->GetComponentLocation();
	PhysicsHandle->GrabComponentAtLocationWithRotation(
		HitComponent,
		NAME_None,// どのボーンを掴むか(スタティックメッシュはNone)
		ComponentLocation,
		HitComponent->GetComponentRotation()
	);
}

void UGrabberComponent::Release()
{
	// ものを掴んでいるか
	if (PhysicsHandle->GetGrabbedComponent() == nullptr) { return; }

	// ものを離す
	PhysicsHandle->ReleaseComponent();
}

//void UGrabberComponent::Look(const FInputActionValue& Value)
//{
//	FVector2D LookAxis = Value.Get<FVector2D>();
//
//	// 左右の視点移動(Yaw)
//	AddControllerYawInput(LookAxis.X);
//
//	// 上下の視点移動(Pitch)
//	AddControllerPitchInput(LookAxis.Y);
//}

void UGrabberComponent::Scroll(const FInputActionValue& Value)
{
	// ものを掴んでいるか
	if (PhysicsHandle->GetGrabbedComponent() == nullptr) { return; }

	// スクロール量を取得して、HoldDistanceを更新
	float ScrollValue = Value.Get<float>();
	HoldDistance += ScrollValue * ScrollSpeed;

	HoldDistance = FMath::Clamp(HoldDistance, MinHoldDistance, MaxHoldDistance);
}