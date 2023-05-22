// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayer/IneractableItem.h"
#include "ExplosiveItem.generated.h"

/**
 * 
 */
UCLASS()
class GETSTARTED_API AExplosiveItem : public AIneractableItem
{
	GENERATED_BODY()
public:
	AExplosiveItem();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Damage")
	float Damage;

public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex) override;
};
