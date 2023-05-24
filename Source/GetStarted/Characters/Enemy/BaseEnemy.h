// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	EEMS_Idle UMETA(DisplayName = "Idle"),
	EEMS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
	EEMS_Attacking UMETA(DisplayName = "Attacking"),
	EEMS_Dead UMETA(DisplayName = "Dead")
};


UCLASS()
class GETSTARTED_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	//AI追踪体积
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AI")
	class USphereComponent* ChaseVolume;

	//攻击范围
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* AttackVolume;

	//AI控制器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	//是否在攻击
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bAttackVolumeOverlapping;

	//动画蒙太奇的引用（只能编辑默认值）在蓝图在使用改变量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackMontage;

	//插值速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float InterpSpeed;

	//是否进行插值,在攻击时为false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bInterpToPlayer;

	//怪物状态
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Status")
	EEnemyMovementStatus EnemyMovementStatus;

	//最大血量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Stats")
	float MaxHealth;

	//当前血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float Health;

	//敌人血条显示组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	class UWidetComponent* HealthBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	class UProgressBar* HealthBar;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void OnChaseVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnChaseVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex);

	UFUNCTION()
	virtual void OnAttackVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnAttackVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex);

	/**
	 * @brief 移动到目标
	 * @param TargetPlayer 玩家
	 */
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(const class AMainPlayer* TargetPlayer);

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
};
