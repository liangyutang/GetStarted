// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerMovementStatus :uint8
{
	EPMS_Normal UMETA(DisplayName="Normal"),
	EPMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EPMS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EPlayerStaminaStatus :uint8
{
	EPSS_Normal UMETA(DisplayName = "Normal"),
	EPSS_Exhausted UMETA(DisplayName = "Exhausted"),
	EPSS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering")
};

UCLASS()
class GETSTARTED_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* FollewCamera;

	float BaseTurnRate;

	float BaseLookUpRate;

	//最大血量
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Player Stats")
	float MaxHealth;

	//当前血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	//最大耐力
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;

	//当前耐力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;

	//耐力消耗速率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float StaminaConsumeRate;

	//疲劳期阈值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float ExhaustedStaminaRatio;

	//耐力状态
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	EPlayerStaminaStatus StaminaStatus;

	//当前的硬币值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int Coins;

	//跑步速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float RunningSpeed;

	//冲刺速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float SprintingSpeed;

	//耐力状态
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	EPlayerMovementStatus MovementStatus;

	//左shift键是否被按下
	bool bLeftShiftKeyDown;

	//是否有武器
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	bool bHasWeapon;

	//当前装备的武器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeaponItem* EquippedWeapon;

	//正在重叠的武器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeaponItem* OverlappingWeapon;

	//攻击键是否被按下
	bool bAttackKeyDown;

	//是否在攻击
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsAttacking;

	//动画蒙太奇的引用（只能编辑默认值）在蓝图在使用改变量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Turn(float Value);

	void LookUp(float Value);

	/**
	 * @brief 旋转角度
	 * @param Rate 
	 */
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(float Value);

	UFUNCTION(BlueprintCallable)
	void IncreaseStamina(float Value);

	UFUNCTION(BlueprintCallable)
	void IncreaseCoin(float Value);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE void LeftShiftKeyDown() { bLeftShiftKeyDown = true; };

	FORCEINLINE void LeftShiftKeyUp() { bLeftShiftKeyDown = false; };

	void SetMovementStatus(EPlayerMovementStatus status);

	void InteractKeyDown();

	void AttackKeyDown();

	//攻击键抬起
	FORCEINLINE void AttackKeyUp() { bAttackKeyDown = false; };

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
};
