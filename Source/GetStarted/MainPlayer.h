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

	//���Ѫ��
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Player Stats")
	float MaxHealth;

	//��ǰѪ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	//�������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;

	//��ǰ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;

	//������������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float StaminaConsumeRate;

	//ƣ������ֵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float ExhaustedStaminaRatio;

	//����״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	EPlayerStaminaStatus StaminaStatus;

	//��ǰ��Ӳ��ֵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int Coins;

	//�ܲ��ٶ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float RunningSpeed;

	//����ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float SprintingSpeed;

	//����״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	EPlayerMovementStatus MovementStatus;

	//��shift���Ƿ񱻰���
	bool bLeftShiftKeyDown;

	//�Ƿ�������
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Weapon")
	bool bHasWeapon;

	//��ǰװ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeaponItem* EquippedWeapon;

	//�����ص�������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeaponItem* OverlappingWeapon;

	//�������Ƿ񱻰���
	bool bAttackKeyDown;

	//�Ƿ��ڹ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsAttacking;

	//������̫������ã�ֻ�ܱ༭Ĭ��ֵ������ͼ��ʹ�øı���
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
	 * @brief ��ת�Ƕ�
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

	//������̧��
	FORCEINLINE void AttackKeyUp() { bAttackKeyDown = false; };

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
};
