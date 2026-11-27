// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "PartTimeCharacterMovement.h"
#include "GrabberComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
// 移動コンポーネントを、独自のクラスに差し替える
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPartTimeCharacterMovement>(ACharacter::CharacterMovementComponentName))
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

