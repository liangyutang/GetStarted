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

	//AI׷�����
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AI")
	class USphereComponent* ChaseVolume;

	//������Χ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* AttackVolume;

	//AI������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	//����״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Status")
	EEnemyMovementStatus EnemyMovementStatus;

	//���Ѫ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Stats")
	float MaxHealth;

	//��ǰѪ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float Health;

	//����Ѫ����ʾ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	class UWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	class UProgressBar* HealthBar;

	//�Ƿ��ڹ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bAttackVolumeOverlapping;

	//������̫������ã�ֻ�ܱ༭Ĭ��ֵ������ͼ��ʹ�øı���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackMontage;

	//��ֵ�ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float InterpSpeed;

	//�Ƿ���в�ֵ,�ڹ���ʱΪfalse;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bInterpToPlayer;

	////�˺���ײ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	class UBoxComponent* LeftAttackCollision;

	//�˺���ײ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	class UBoxComponent* RightAttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage;

	//����ͼ��ָ��,�˺�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> DamageTypeClass;

	//����ʱ���ŵ�����Ч��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Effect")
	UParticleSystem* HitParticles;

	//����ʱ���ŵ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Effect")
	class USoundCue* HitSound;

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

	UFUNCTION()
	virtual void OnLeftAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnLeftAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex);

	UFUNCTION()
	virtual void OnRightAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnRightAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex);

	//����LeftAttackCollision��ײ
	UFUNCTION(BlueprintCallable)
	void ActiveLeftAttackCollision();

	//�ر�LeftAttackCollision��ײ
	UFUNCTION(BlueprintCallable)
	void DeactiveLeftAttackCollision();

	//����RightAttackCollision��ײ
	UFUNCTION(BlueprintCallable)
	void ActiveRightAttackCollision();

	//�ر�RightAttackCollision��ײ
	UFUNCTION(BlueprintCallable)
	void DeactiveRightAttackCollision();

	/**
	 * @brief �ƶ���Ŀ��
	 * @param TargetPlayer ���
	 */
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(const class AMainPlayer* TargetPlayer);

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	FORCEINLINE bool IsAlive() const { return EnemyMovementStatus != EEnemyMovementStatus::EEMS_Dead; }

	bool HasValidTarget();
};
