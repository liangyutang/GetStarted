// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayer/IneractableItem.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class GETSTARTED_API AWeaponItem : public AIneractableItem
{
	GENERATED_BODY()


public:

	AWeaponItem();

	//��������
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Sound")
	USoundCue* OnEquippedSound;

	//Idle����Ч���Ƿ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Particle")
	bool bShouldReserveIdleParticle;

protected:

	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex) override;

	//װ������
	void Equip();

	//ж������
	void UnEquip();
};
