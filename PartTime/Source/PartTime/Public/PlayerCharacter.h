// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UPhysicsHandleComponent;

class UGrabberComponent;

UCLASS()
class PARTTIME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);



private:
	// 一人称カメラ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera = nullptr;

	// 物理グラブフィジックスハンドル
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGrabberComponent> Grabber = nullptr;
};
