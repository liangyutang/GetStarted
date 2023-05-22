// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	Super::Jump();
}

void AMainPlayer::MoveForward(float Value)
{
	//移动1
	//AddMovementInput(GetActorForwardVector(), Value);

	if ((Controller != nullptr) && (Value != 0.0f))
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

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FRotator Rotator = Controller->GetControlRotation();
		FRotator YawRotator(0.0f, Rotator.Yaw, 0.0f);
		FVector Direction = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::Turn(float Value)
{
	if (Value!=0.0f)
	{
		AddControllerYawInput(Value);
	}
}

void AMainPlayer::LookUp(float Value)
{
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
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value);
	}
}

void AMainPlayer::LookUpAtRate(float Rate)
{
	float Value = Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
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
		//TODO Die();
	}
	else
	{
		Health -= Damage;
	}

	return Health;
}

