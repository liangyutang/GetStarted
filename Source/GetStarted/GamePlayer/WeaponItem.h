// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayer/IneractableItem.h"
#include "WeaponItem.generated.h"

UENUM(BlueprintType)
enum class EWeaponState:uint8
{
	//�ڵ���
	EWS_CanPickup UMETA(DisplayName="CanPickup"),
	//�����ʹ��
	EWS_Equipped UMETA(DisplayName = "Equipped")
};

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

	//����״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponState WeaponState;

	//�˺���ײ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Weapon|Attack")
	class UBoxComponent* AttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Attack")
	float Damage;

	//����ͼ��ָ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Attack")
	TSubclassOf<UDamageType> DamageTypeClass;

	//����ӵ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Attack")
	AController* WeaponOwner;

protected:

	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex) override;

	//װ������
	void Equip(class AMainPlayer* MainPlayer);

	//ж������
	void UnEquip(AMainPlayer* MainPlayer);

	//����DisplayMesh��ײ
	void ActiveDisplayMeshCollision();

	//�ر�DisplayMesh��ײ
	void DeactiveDisplayMeshCollision();

	UFUNCTION()
	virtual void OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex);

	//����AttackCollision��ײ
	UFUNCTION(BlueprintCallable)
	void ActiveAttackCollisionCollision();

	//�ر�AttackCollision��ײ
	UFUNCTION(BlueprintCallable)
	void DeactiveAttackCollisionCollision();
};
