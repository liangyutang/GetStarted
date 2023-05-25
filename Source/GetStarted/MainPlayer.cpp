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

	//设置摄像机
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	//距离人身长度
	SpringArm->TargetArmLength = 600.0f;
	//开启视角左右移动
	SpringArm->bUsePawnControlRotation = true;

	//设置支架臂
	FollewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollewCamera"));
	FollewCamera->SetupAttachment(SpringArm,USpringArmComponent::SocketName);
	//开启视角左右移动
	FollewCamera->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCapsuleSize(35.0f, 100.f);

	//关闭角色方向移动，使摄像机可以看到角色的脸，但使用“移动1”，会导致角色的移动方向与视角不一致
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//控制角色转向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//旋转速度
	GetCharacterMovement()->RotationRate=FRotator(0.0f, 500.0f, 0.0f);
	//跳跃速度（高度）
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	//角色在控制的控制力度，数值越大，角色在控制的控制性就越强
	GetCharacterMovement()->AirControl = 0.15f;

	//手柄或键盘上下键视角移动速度
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
			//冲刺键按下
			if (Stamina-StaminaConsumeRate* DeltaTime<=MaxStamina*ExhaustedStaminaRatio)
			{
				//下一帧疲惫状态
				StaminaStatus = EPlayerStaminaStatus::EPSS_Exhausted;
				
			}
			Stamina -= StaminaConsumeRate * DeltaTime;
			//当前帧为冲刺状态
			SetMovementStatus(EPlayerMovementStatus::EPMS_Sprinting);
			
		}
		else
		{
			//冲刺键抬起，状态不变
			Stamina = FMath::Clamp(Stamina + StaminaConsumeRate * DeltaTime, 0.0f, MaxStamina);
			SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
		}
		break;
	case EPlayerStaminaStatus::EPSS_ExhaustedRecovering:
		if (Stamina - StaminaConsumeRate * DeltaTime >= MaxStamina * ExhaustedStaminaRatio)
		{
			//恢复后，耐力值不在疲劳区
			StaminaStatus = EPlayerStaminaStatus::EPSS_Normal;
		}
		//恢复耐力
		Stamina += StaminaConsumeRate * DeltaTime;
		LeftShiftKeyUp();
		SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
		break;
	case EPlayerStaminaStatus::EPSS_Exhausted:
		if (bLeftShiftKeyDown)
		{
			//冲刺键按下
			if (Stamina - StaminaConsumeRate * DeltaTime <= 0.0f)
			{
				//没有耐力了
				StaminaStatus = EPlayerStaminaStatus::EPSS_ExhaustedRecovering;
				LeftShiftKeyUp();
				SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
			}
			else
			{
				//还有一些耐力
				Stamina -= StaminaConsumeRate * DeltaTime;
			}

		}
		else
		{
			//冲刺键抬起
			StaminaStatus = EPlayerStaminaStatus::EPSS_ExhaustedRecovering;
			Stamina = FMath::Clamp(Stamina + StaminaConsumeRate * DeltaTime, 0.0f, MaxStamina);
			SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
		}
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
		break;
	}

	//使用玩家面向敌人
	//不在攻击状态下，且敌人不为空
	if (bInterpToEnemy && AttackTarget)
	{
		//获得玩家对敌人的	朝向
		const FRotator LookAtYaw(0.0f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation()).Yaw,0.0f);
		//面向敌人
		const FRotator InterpRotation=FMath::RInterpTo(GetActorRotation(),LookAtYaw,DeltaTime,InterpSpeed);
		SetActorRotation(InterpRotation);
	}

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//检查是否为空，若为空，则不运行下面的代码（check最好只在该方法中运行）
	check(PlayerInputComponent);

	//跳跃
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//F键
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::InteractKeyDown);

	//冲刺
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainPlayer::LeftShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainPlayer::LeftShiftKeyUp);

	//攻击
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainPlayer::AttackKeyDown);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMainPlayer::AttackKeyUp);

	//前进与后退
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	//左右
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	//旋转
	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPlayer::LookUp);

	//旋转角度
	PlayerInputComponent->BindAxis("TurnAtRate", this, &AMainPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &AMainPlayer::LookUpAtRate);

	
}

void AMainPlayer::Jump()
{

	//攻击时，不允许跳跃
	if (!bIsAttacking && IsAlive())
	{
		Super::Jump();
	}
}

void AMainPlayer::MoveForward(float Value)
{
	//移动1
	//AddMovementInput(GetActorForwardVector(), Value);

	//攻击时不可移动
	if ((Controller != nullptr) && (Value != 0.0f)&&(!bIsAttacking) && IsAlive())
	{
		//获取摄像机的角度
		FRotator Rotator = Controller->GetControlRotation();
		//获取旋转的向量
		FRotator YawRotator(0.0f, Rotator.Yaw, 0.0f);
		//计算X轴的旋转向量
		FVector Direction = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		//移动
		AddMovementInput(Direction, Value);
	}
	
}

void AMainPlayer::MoveRight(float Value)
{

	//移动1
	//AddMovementInput(GetActorRightVector(), Value);


	//攻击时不可移动
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

	//只允许视角在0-45度，270-360度之间（视角正向面对角色背部时，视角度数为0或360度）
	//当视角在180-270度之间，但是玩家仍然向上抬视角，则不被运行，若向下，则允许
	if (GetControlRotation().Pitch<270.0f&&GetControlRotation().Pitch>180&&Value>0.0f)
	{
		return;
	}
	//当视角在45-180度之间，但是玩家仍然向下抬视角，则不被运行，若向上，则允许
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

	//只允许视角在0-45度，270-360度之间（视角正向面对角色背部时，视角度数为0或360度）
	//当视角在180-270度之间，但是玩家仍然向上抬视角，则不被运行，若向下，则允许
	if (GetControlRotation().Pitch < 270.0f && GetControlRotation().Pitch>180 && Value > 0.0f)
	{
		return;
	}
	//当视角在45-180度之间，但是玩家仍然向下抬视角，则不被运行，若向上，则允许
	if (GetControlRotation().Pitch < 180.0f && GetControlRotation().Pitch>45.0f && Value < 0.0f)
	{
		return;
	}

	AddControllerPitchInput(Value);
}

//增加血量，减血用引擎自带方法来减
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
	//减血
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


	//是否有武器在范围内
	if (OverlappingWeapon)
	{
		//范围内有武器,手中是否有武器
		if (EquippedWeapon)
		{
			//手中有武器
			EquippedWeapon->UnEquip(this);
			OverlappingWeapon->Equip(this);
		}
		else
		{
			//手中没武器，装备武器
			OverlappingWeapon->Equip(this);
		}
		
	}
	else
	{
		//范围内没有武器，这里扔掉手中的武器(手中没有武器，则不做任何操作)
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
		//有武器，攻击，是否在攻击间隔外，在Attack()中判断
		Attack();
	}
}

void AMainPlayer::Attack()
{
	//在空中或者攻击时，不会调用
	if (!bIsAttacking && !(GetMovementComponent()->IsFalling())&& IsAlive())
	{
		//当前没有在攻击
		bIsAttacking = true;
		bInterpToEnemy = true;

		//获取动画实例
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			//获取随机播放速率
			const float PlayRate = FMath::RandRange(1.25f, 1.75f);
			//获取随机section
			const FString SectionName = FString::FromInt(FMath::RandRange(1, 2));

			//用那种速率播放那个蒙太奇
			AnimInstance->Montage_Play(AttackMontage,PlayRate);
			//播放蒙太奇的那个section
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

	//攻击键仍然被按着
	if (bAttackKeyDown)
	{
		AttackKeyDown();
	}
}

/**
 * @brief 寻找最近的敌人为攻击目标
 */
void AMainPlayer::UpdateAttackTarget()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	//看看范围内是否有敌人
	if (OverlappingActors.Num()==0)
	{
		AttackTarget = nullptr;
		return;
	}

	ABaseEnemy* CloseEnemy = nullptr;

	//最小距离
	float MinDistance = 1000.0f;

	const FVector Location = GetActorLocation();

	for (const auto Actor:OverlappingActors)
	{
		ABaseEnemy* Enemy = Cast<ABaseEnemy>(Actor);
		//敌人存在，且未死亡
		if (Enemy&&Enemy->EnemyMovementStatus!=EEnemyMovementStatus::EEMS_Dead)
		{
			//计算距离
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
	//是否装备武器
	if (EquippedWeapon)
	{
		//消除碰撞
		EquippedWeapon->DeactiveAttackCollision();
		EquippedWeapon->DeactiveDisplayMeshCollision();
	}
}

void AMainPlayer::DeathEnd()
{
	//禁用所有动画相关
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	//1秒后，调用RestartLevel()
	FTimerHandle DeathTimerHandle;
	auto Lambda = [this]()
	{
		//TODO RestartLevel();
	};

	GetWorldTimerManager().SetTimer(DeathTimerHandle, FTimerDelegate::CreateLambda(Lambda), 1.0f, false);
}

