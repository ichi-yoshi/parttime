#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.h"
#include "BasketActor.generated.h"

UCLASS()
class PARTTIME_API ABasketActor : public AActor
{
	GENERATED_BODY()

public:
	ABasketActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basket", meta = (ExposeOnSpawn = "true"))
	FString BasketID = TEXT("A");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basket")
	TMap<int32,TSubclassOf<AItemActor>> ItemActorClassMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basket")
	TArray<FVector> ItemSlotOffsets;

	UFUNCTION(BlueprintCallable, Category = "Basket")
	void SpawnItemsInBasket(const TArray<int32>& ItemIDs);

	UFUNCTION(BlueprintCallable, Category = "Basket")
	void ClearBasket();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basket")
	UStaticMeshComponent* BasketMesh;

private:
	UPROPERTY()
	TArray<AItemActor*> SpawnedItemActors;
};
