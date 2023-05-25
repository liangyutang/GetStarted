// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GamePlayer/WeaponItem.h"
#include "GetStarted/Characters/Enemy/BaseEnemy.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���������
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	//����������
	SpringArm->TargetArmLength = 600.0f;
	//�����ӽ������ƶ�
	SpringArm->bUsePawnControlRotation = true;

	//����֧�ܱ�
	FollewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollewCamera"));
	FollewCamera->SetupAttachment(SpringArm,USpringArmComponent::SocketName);
	//�����ӽ������ƶ�
	FollewCamera->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCapsuleSize(35.0f, 100.f);

	//�رս�ɫ�����ƶ���ʹ��������Կ�����ɫ��������ʹ�á��ƶ�1�����ᵼ�½�ɫ���ƶ��������ӽǲ�һ��
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//���ƽ�ɫת��
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//��ת�ٶ�
	GetCharacterMovement()->RotationRate=FRotator(0.0f, 500.0f, 0.0f);
	//��Ծ�ٶȣ��߶ȣ�
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	//��ɫ�ڿ��ƵĿ������ȣ���ֵԽ�󣬽�ɫ�ڿ��ƵĿ����Ծ�Խǿ
	GetCharacterMovement()->AirControl = 0.15f;

	//�ֱ���������¼��ӽ��ƶ��ٶ�
	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;

	MaxHealth = 100.0f;
	Health = MaxHealth;

	MaxStamina = 150.0f;
	Stamina = MaxStamina;
	StaminaConsumeRate = 20.0f;
	ExhaustedStaminaRatio = 0.167f;
	StaminaStatus = EPlayerStaminaStatus::EPSS_Normal;

	Coins = 0;

	RunningSpeed = 600.0f;
	SprintingSpeed = 900.0f;
	MovementStatus = EPlayerMovementStatus::EPMS_Normal;
	bLeftShiftKeyDown = false;

	bAttackKeyDown = false;
	bIsAttacking = false;

	AttackTarget = nullptr;
	InterpSpeed = 15.0f;
	bInterpToEnemy = false;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsAlive())
	{
		return;
	}

	switch (StaminaStatus)
	{
	case EPlayerStaminaStatus::EPSS_Normal:
		if (bLeftShiftKeyDown)
		{
			//��̼�����
			if (Stamina-StaminaConsumeRate* DeltaTime<=MaxStamina*ExhaustedStaminaRatio)
			{
				//��һ֡ƣ��״̬
				StaminaStatus = EPlayerStaminaStatus::EPSS_Exhausted;
				
			}
			Stamina -= StaminaConsumeRate * DeltaTime;
			//��ǰ֡Ϊ���״̬
			SetMovementStatus(EPlayerMovementStatus::EPMS_Sprinting);
			
		}
		else
		{
			//��̼�̧��״̬����
			Stamina = FMath::Clamp(Stamina + StaminaConsumeRate * DeltaTime, 0.0f, MaxStamina);
			SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
		}
		break;
	case EPlayerStaminaStatus::EPSS_ExhaustedRecovering:
		if (Stamina - StaminaConsumeRate * DeltaTime >= MaxStamina * ExhaustedStaminaRatio)
		{
			//�ָ�������ֵ����ƣ����
			StaminaStatus = EPlayerStaminaStatus::EPSS_Normal;
		}
		//�ָ�����
		Stamina += StaminaConsumeRate * DeltaTime;
		LeftShiftKeyUp();
		SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
		break;
	case EPlayerStaminaStatus::EPSS_Exhausted:
		if (bLeftShiftKeyDown)
		{
			//��̼�����
			if (Stamina - StaminaConsumeRate * DeltaTime <= 0.0f)
			{
				//û��������
				StaminaStatus = EPlayerStaminaStatus::EPSS_ExhaustedRecovering;
				LeftShiftKeyUp();
				SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
			}
			else
			{
				//����һЩ����
				Stamina -= StaminaConsumeRate * DeltaTime;
			}

		}
		else
		{
			//��̼�̧��
			StaminaStatus = EPlayerStaminaStatus::EPSS_ExhaustedRecovering;
			Stamina = FMath::Clamp(Stamina + StaminaConsumeRate * DeltaTime, 0.0f, MaxStamina);
			SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
		}
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
		break;
	}

	//ʹ������������
	//���ڹ���״̬�£��ҵ��˲�Ϊ��
	if (bInterpToEnemy && AttackTarget)
	{
		//�����ҶԵ��˵�	����
		const FRotator LookAtYaw(0.0f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation()).Yaw,0.0f);
		//�������
		const FRotator InterpRotation=FMath::RInterpTo(GetActorRotation(),LookAtYaw,DeltaTime,InterpSpeed);
		SetActorRotation(InterpRotation);
	}

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//����Ƿ�Ϊ�գ���Ϊ�գ�����������Ĵ��루check���ֻ�ڸ÷��������У�
	check(PlayerInputComponent);

	//��Ծ
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//F��
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::InteractKeyDown);

	//���
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainPlayer::LeftShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainPlayer::LeftShiftKeyUp);

	//����
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainPlayer::AttackKeyDown);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMainPlayer::AttackKeyUp);

	//ǰ�������
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	//����
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	//��ת
	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPlayer::LookUp);

	//��ת�Ƕ�
	PlayerInputComponent->BindAxis("TurnAtRate", this, &AMainPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &AMainPlayer::LookUpAtRate);

	
}

void AMainPlayer::Jump()
{

	//����ʱ����������Ծ
	if (!bIsAttacking && IsAlive())
	{
		Super::Jump();
	}
}

void AMainPlayer::MoveForward(float Value)
{
	//�ƶ�1
	//AddMovementInput(GetActorForwardVector(), Value);

	//����ʱ�����ƶ�
	if ((Controller != nullptr) && (Value != 0.0f)&&(!bIsAttacking) && IsAlive())
	{
		//��ȡ������ĽǶ�
		FRotator Rotator = Controller->GetControlRotation();
		//��ȡ��ת������
		FRotator YawRotator(0.0f, Rotator.Yaw, 0.0f);
		//����X�����ת����
		FVector Direction = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		//�ƶ�
		AddMovementInput(Direction, Value);
	}
	
}

void AMainPlayer::MoveRight(float Value)
{

	//�ƶ�1
	//AddMovementInput(GetActorRightVector(), Value);


	//����ʱ�����ƶ�
	if ((Controller != nullptr) && (Value != 0.0f)&&(!bIsAttacking) && IsAlive())
	{
		FRotator Rotator = Controller->GetControlRotation();
		FRotator YawRotator(0.0f, Rotator.Yaw, 0.0f);
		FVector Direction = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::Turn(float Value)
{
	if (Value!=0.0f && IsAlive())
	{
		AddControllerYawInput(Value);
	}
}

void AMainPlayer::LookUp(float Value)
{
	if (!IsAlive())
	{
		return;
	}

	//ֻ�����ӽ���0-45�ȣ�270-360��֮�䣨�ӽ�������Խ�ɫ����ʱ���ӽǶ���Ϊ0��360�ȣ�
	//���ӽ���180-270��֮�䣬���������Ȼ����̧�ӽǣ��򲻱����У������£�������
	if (GetControlRotation().Pitch<270.0f&&GetControlRotation().Pitch>180&&Value>0.0f)
	{
		return;
	}
	//���ӽ���45-180��֮�䣬���������Ȼ����̧�ӽǣ��򲻱����У������ϣ�������
	if (GetControlRotation().Pitch < 180.0f && GetControlRotation().Pitch>45.0f && Value < 0.0f)
	{
		return;
	}

	AddControllerPitchInput(Value);
}

void AMainPlayer::TurnAtRate(float Rate)
{
	float Value = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	if (Value != 0.0f && IsAlive())
	{
		AddControllerYawInput(Value);
	}
}

void AMainPlayer::LookUpAtRate(float Rate)
{
	const float Value = Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
	if (!IsAlive())
	{
		return;
	}
	
	//if (Value != 0.0f)
	//{
	//	AddControllerPitchInput(Value);
	//}

	//ֻ�����ӽ���0-45�ȣ�270-360��֮�䣨�ӽ�������Խ�ɫ����ʱ���ӽǶ���Ϊ0��360�ȣ�
	//���ӽ���180-270��֮�䣬���������Ȼ����̧�ӽǣ��򲻱����У������£�������
	if (GetControlRotation().Pitch < 270.0f && GetControlRotation().Pitch>180 && Value > 0.0f)
	{
		return;
	}
	//���ӽ���45-180��֮�䣬���������Ȼ����̧�ӽǣ��򲻱����У������ϣ�������
	if (GetControlRotation().Pitch < 180.0f && GetControlRotation().Pitch>45.0f && Value < 0.0f)
	{
		return;
	}

	AddControllerPitchInput(Value);
}

//����Ѫ������Ѫ�������Դ���������
void AMainPlayer::IncreaseHealth(float Value)
{
	Health = FMath::Clamp(Health + Value, 0.0f, MaxHealth);
}

void AMainPlayer::IncreaseStamina(float Value)
{
	Stamina = FMath::Clamp(Stamina + Value, 0.0f, MaxStamina);
}

void AMainPlayer::IncreaseCoin(float Value)
{
	Coins += Value;
}

float AMainPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	//��Ѫ
	if (Health-Damage<=0.0f)
	{
		Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
		Die();
	}
	else
	{
		Health -= Damage;
	}

	return Health;
}

void AMainPlayer::SetMovementStatus(EPlayerMovementStatus status)
{
	if (!IsAlive())
	{
		return;
	}


	MovementStatus = status;
	switch (MovementStatus)
	{
	case EPlayerMovementStatus::EPMS_Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		break;

	default:
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
		break;
	}
}

void AMainPlayer::InteractKeyDown()
{
	if (!IsAlive())
	{
		return;
	}


	//�Ƿ��������ڷ�Χ��
	if (OverlappingWeapon)
	{
		//��Χ��������,�����Ƿ�������
		if (EquippedWeapon)
		{
			//����������
			EquippedWeapon->UnEquip(this);
			OverlappingWeapon->Equip(this);
		}
		else
		{
			//����û������װ������
			OverlappingWeapon->Equip(this);
		}
		
	}
	else
	{
		//��Χ��û�������������ӵ����е�����(����û�������������κβ���)
		if (EquippedWeapon)
		{
			EquippedWeapon->UnEquip(this);
		}
	}
}

void AMainPlayer::AttackKeyDown()
{
	if (!IsAlive())
	{
		return;
	}


	bAttackKeyDown = true;

	if (bHasWeapon)
	{
		//���������������Ƿ��ڹ�������⣬��Attack()���ж�
		Attack();
	}
}

void AMainPlayer::Attack()
{
	//�ڿ��л��߹���ʱ���������
	if (!bIsAttacking && !(GetMovementComponent()->IsFalling())&& IsAlive())
	{
		//��ǰû���ڹ���
		bIsAttacking = true;
		bInterpToEnemy = true;

		//��ȡ����ʵ��
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			//��ȡ�����������
			const float PlayRate = FMath::RandRange(1.25f, 1.75f);
			//��ȡ���section
			const FString SectionName = FString::FromInt(FMath::RandRange(1, 2));

			//���������ʲ����Ǹ���̫��
			AnimInstance->Montage_Play(AttackMontage,PlayRate);
			//������̫����Ǹ�section
			AnimInstance->Montage_JumpToSection(FName(*SectionName),AttackMontage);
		}
	}
}

void AMainPlayer::AttackEnd()
{
	bIsAttacking = false;
	bInterpToEnemy = false;

	if (!IsAlive())
	{
		return;
	}

	//��������Ȼ������
	if (bAttackKeyDown)
	{
		AttackKeyDown();
	}
}

/**
 * @brief Ѱ������ĵ���Ϊ����Ŀ��
 */
void AMainPlayer::UpdateAttackTarget()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	//������Χ���Ƿ��е���
	if (OverlappingActors.Num()==0)
	{
		AttackTarget = nullptr;
		return;
	}

	ABaseEnemy* CloseEnemy = nullptr;

	//��С����
	float MinDistance = 1000.0f;

	const FVector Location = GetActorLocation();

	for (const auto Actor:OverlappingActors)
	{
		ABaseEnemy* Enemy = Cast<ABaseEnemy>(Actor);
		//���˴��ڣ���δ����
		if (Enemy&&Enemy->EnemyMovementStatus!=EEnemyMovementStatus::EEMS_Dead)
		{
			//�������
			const float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
			if (DistanceToActor<MinDistance)
			{
				MinDistance = DistanceToActor;
				CloseEnemy = Enemy;
			}
		}
	}

	AttackTarget = CloseEnemy;
}

void AMainPlayer::Die()
{
	SetMovementStatus(EPlayerMovementStatus::EPMS_Dead);
	//�Ƿ�װ������
	if (EquippedWeapon)
	{
		//������ײ
		EquippedWeapon->DeactiveAttackCollision();
		EquippedWeapon->DeactiveDisplayMeshCollision();
	}
}

void AMainPlayer::DeathEnd()
{
	//�������ж������
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	//1��󣬵���RestartLevel()
	FTimerHandle DeathTimerHandle;
	auto Lambda = [this]()
	{
		//TODO RestartLevel();
	};

	GetWorldTimerManager().SetTimer(DeathTimerHandle, FTimerDelegate::CreateLambda(Lambda), 1.0f, false);
}

