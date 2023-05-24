// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayer/IneractableItem.h"
#include "WeaponItem.generated.h"

UENUM(BlueprintType)
enum class EWeaponState:uint8
{
	//在地上
	EWS_CanPickup UMETA(DisplayName="CanPickup"),
	//被玩家使用
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

	//武器声音
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon|Sound")
	USoundCue* OnEquippedSound;

	//Idle粒子效果是否保留
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Particle")
	bool bShouldReserveIdleParticle;

	//武器状态
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponState WeaponState;

	//伤害碰撞
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Weapon|Attack")
	class UBoxComponent* AttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Attack")
	float Damage;

	//在蓝图中指定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Attack")
	TSubclassOf<UDamageType> DamageTypeClass;

	//武器拥有者
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Attack")
	AController* WeaponOwner;

protected:

	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex) override;

	//装备武器
	void Equip(class AMainPlayer* MainPlayer);

	//卸下武器
	void UnEquip(AMainPlayer* MainPlayer);

	//激活DisplayMesh碰撞
	void ActiveDisplayMeshCollision();

	//关闭DisplayMesh碰撞
	void DeactiveDisplayMeshCollision();

	UFUNCTION()
	virtual void OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex);

	//激活AttackCollision碰撞
	UFUNCTION(BlueprintCallable)
	void ActiveAttackCollisionCollision();

	//关闭AttackCollision碰撞
	UFUNCTION(BlueprintCallable)
	void DeactiveAttackCollisionCollision();
};
