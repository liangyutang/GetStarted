// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayer/IneractableItem.h"
#include "PickupItem.generated.h"

/**
 * 
 */
UCLASS()
class GETSTARTED_API APickupItem : public AIneractableItem
{
	GENERATED_BODY()
	
public:
	APickupItem();

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnPickup(AMainPlayer* MainPlayer);
};
