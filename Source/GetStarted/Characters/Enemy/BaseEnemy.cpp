// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseEnemy.h"

#include "AIController.h"
#include "MainPlayer.h"
#include "ProgressBar.h"
#include "WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/**如果怪物挡在摄像机与玩家之间，需要将怪物处理*/
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	ChaseVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ChaseVolume"));
	ChaseVolume->SetupAttachment(GetRootComponent());
	ChaseVolume->InitSphereRadius(800.0f);
	//设置碰撞检测
	//指定当前的碰撞类型
	ChaseVolume->SetCollisionObjectType(ECC_WorldDynamic);
	//只响应pawn，其他不响应,不知道为什么，AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);不生效
	ChaseVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	ChaseVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	

	AttackVolume = CreateDefaultSubobject<USphereComponent>(TEXT("AttackVolume"));
	AttackVolume->SetupAttachment(GetRootComponent());
	AttackVolume->InitSphereRadius(100.0f);
	//设置碰撞检测
	//指定当前的碰撞类型
	AttackVolume->SetCollisionObjectType(ECC_WorldDynamic);
	//只响应pawn，其他不响应,不知道为什么，AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);不生效
	AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	HealthBarWidgetComponent= CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	//在玩家屏幕上显示
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	//大小
	HealthBarWidgetComponent->SetDrawSize(FVector2D(125.0f, 10.0f));


	//设置AI
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//设置AI状态为游走状态
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;
	MaxHealth = 100.0f;
	Health = MaxHealth;

	bAttackVolumeOverlapping = false;
	InterpSpeed = 15.0f;
	bInterpToPlayer = false;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	//设置碰撞检测
	//指定当前的碰撞类型
	ChaseVolume->SetCollisionObjectType(ECC_WorldDynamic);
	//只响应pawn，其他不响应,不知道为什么，AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);不生效
	ChaseVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	ChaseVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//设置碰撞检测
	//指定当前的碰撞类型
	AttackVolume->SetCollisionObjectType(ECC_WorldDynamic);
	//只响应pawn，其他不响应,不知道为什么，AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);不生效
	AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//当区域开始被重叠时
	ChaseVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnChaseVolumeOverlapBegin);
	//当重叠事件结束时
	ChaseVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnChaseVolumeOverlapEnd);

	//当区域开始被重叠时
	AttackVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnAttackVolumeOverlapBegin);
	//当重叠事件结束时
	AttackVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnAttackVolumeOverlapEnd);

	//获取血量条对应的组件
	HealthBar= Cast<UProgressBar>(HealthBarWidgetComponent->GetUserWidgetObject()->GetWidgetFromName("HealthBar"));
	//设置百分比
	HealthBar->SetPercent(Health / MaxHealth);
	//刚开始隐藏
	HealthBar->SetVisibility(ESlateVisibility::Hidden);

	AIController = Cast<AAIController>(GetController());
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//使用玩家面向敌人
	//不在攻击状态下
	if (bInterpToPlayer)
	{
		//获得玩家对敌人的	朝向(获取0号玩家：GameplayStatics::GetPlayerPawn(this,0))
		const FRotator LookAtYaw(0.0f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UGameplayStatics::GetPlayerPawn(this,0)->GetActorLocation()).Yaw, 0.0f);
		//面向敌人
		const FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);
		SetActorRotation(InterpRotation);
	}

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::OnChaseVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		const AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			HealthBar->SetVisibility(ESlateVisibility::Visible);
			//追逐玩家
			MoveToTarget(MainPlayer);
		}
	}
}

void ABaseEnemy::OnChaseVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		const AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			/**停止追逐*/
			EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;

			HealthBar->SetVisibility(ESlateVisibility::Hidden);

			if (AIController)
			{
				//停止移动
				AIController->StopMovement();
			}
		}
	}
}

void ABaseEnemy::OnAttackVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			//更新玩家的攻击目标
			MainPlayer->UpdateAttackTarget();

			bAttackVolumeOverlapping = true;
			Attack();
		}
	}
}

void ABaseEnemy::OnAttackVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		const AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			bAttackVolumeOverlapping = false;
			//是否攻击完成
			if (EnemyMovementStatus!=EEnemyMovementStatus::EEMS_Attacking)
			{
				MoveToTarget(MainPlayer);
			}
		}
	}
}

void ABaseEnemy::MoveToTarget(const AMainPlayer* TargetPlayer)
{
	//设置为追逐状态
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_MoveToTarget;

	if (AIController)
	{
		//准备参数
		FAIMoveRequest MoveRequest;
		//设置追逐目标
		MoveRequest.SetGoalActor(TargetPlayer);
		//距离敌人多进借书追逐
		MoveRequest.SetAcceptanceRadius(10.0f);

		//导航路径
		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		//将导航路径渲染出来
		/*auto PathPoints= NavPath->GetPathPoints();

	    for (auto Point:PathPoints)
		{
			FVector Location = Point.Location;

			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.0f, 8, FLinearColor::Red, 10.0f, 1.5f);
		}*/

	}
}

void ABaseEnemy::Attack()
{
	if (AIController)
	{
		AIController->StopMovement();
	}
	//看现在是否在攻击
	if (EnemyMovementStatus!=EEnemyMovementStatus::EEMS_Attacking)
	{
		EnemyMovementStatus = EEnemyMovementStatus::EEMS_Attacking;

		bInterpToPlayer = true;

		//获取动画实例
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			//获取随机播放速率
			const float PlayRate = FMath::RandRange(0.9f, 1.1f);
			//获取随机section
			const FString SectionName = FString::FromInt(FMath::RandRange(1, 3));

			//用那种速率播放那个蒙太奇
			AnimInstance->Montage_Play(AttackMontage, PlayRate);
			//播放蒙太奇的那个section
			AnimInstance->Montage_JumpToSection(FName(*SectionName), AttackMontage);
		}
	}
}

void ABaseEnemy::AttackEnd()
{
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;

	bInterpToPlayer = false;

	if (bAttackVolumeOverlapping)
	{
		Attack();
	}
}

