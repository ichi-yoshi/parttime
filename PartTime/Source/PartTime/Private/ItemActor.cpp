#include "ItemActor.h"
#include "Components/StaticMeshComponent.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// メッシュをルートにする。物理で動いたときにアクターの位置も一緒に動くように
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	// 物理シミュレーションを有効にし、商品用コリジョンプリセットを使う
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName(TEXT("Item"));

	// キャラクターが段差として商品に乗らないようにする
	Mesh->CanCharacterStepUpOn = ECB_No;
}

void AItemActor::ApplyItemData_Implementation(const FItemData& NewData)
{
	ItemData = NewData;
}
