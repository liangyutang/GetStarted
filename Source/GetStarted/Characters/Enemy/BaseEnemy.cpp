// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseEnemy.h"

#include "AIController.h"
#include "MainPlayer.h"
#include "ProgressBar.h"
#include "WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
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

	LeftAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftAttackCollision"));
	LeftAttackCollision->SetupAttachment(GetMesh(), "LeftAttackSocket");
	//LeftAttackCollision->SetupAttachment(GetRootComponent());
	DeactiveLeftAttackCollision();
	LeftAttackCollision->SetRelativeScale3D(FVector(2.5f, 0.5f, 0.25f));
	LeftAttackCollision->SetRelativeLocation(FVector(-34.0f, 0.0f, 0.0f));

	RightAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightAttackCollision"));
	RightAttackCollision->SetupAttachment(GetMesh(), "RightAttackSocket");
	//RightAttackCollision->SetupAttachment(GetRootComponent());
	DeactiveRightAttackCollision();
	RightAttackCollision->SetRelativeScale3D(FVector(2.5f, 0.5f, 0.25f));
	RightAttackCollision->SetRelativeLocation(FVector(34.0f, 0.0f, 0.0f));

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

	Damage = 10.0f;
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

	//当区域开始被重叠时
	LeftAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnLeftAttackCollisionOverlapBegin);
	//当重叠事件结束时
	LeftAttackCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnLeftAttackCollisionOverlapEnd);

	//当区域开始被重叠时
	RightAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnRightAttackCollisionOverlapBegin);
	//当重叠事件结束时
	RightAttackCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnRightAttackCollisionOverlapEnd);

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

void ABaseEnemy::OnLeftAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			//生成粒子效果
			if (MainPlayer->HitParticles)
			{
				//在那个位置生成
				const USkeletalMeshSocket* AttackSocket =GetMesh()->GetSocketByName("LeftAttackSocket");
				if (AttackSocket)
				{
					const FVector SocketLocation = AttackSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(this, MainPlayer->HitParticles, SocketLocation, FRotator(0.0f), true);
				}
			}

			//生成声音
			if (MainPlayer->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, MainPlayer->HitSound);
			}

			//施加伤害
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(MainPlayer, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void ABaseEnemy::OnLeftAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseEnemy::OnRightAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			//生成粒子效果
			if (MainPlayer->HitParticles)
			{
				//在那个位置生成
				const USkeletalMeshSocket* AttackSocket = GetMesh()->GetSocketByName("RightAttackSocket");
				if (AttackSocket)
				{
					const FVector SocketLocation = AttackSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(this, MainPlayer->HitParticles, SocketLocation, FRotator(0.0f), true);
				}
			}

			//生成声音
			if (MainPlayer->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, MainPlayer->HitSound);
			}

			//施加伤害
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(MainPlayer, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void ABaseEnemy::OnRightAttackCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseEnemy::ActiveLeftAttackCollision()
{
	//设置触发器的碰撞检测
	//设置体积的状态-查询与物理
	LeftAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//指定当前的碰撞类型
	LeftAttackCollision->SetCollisionObjectType(ECC_WorldDynamic);
	//只响应pawn，其他不响应
	LeftAttackCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftAttackCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABaseEnemy::DeactiveLeftAttackCollision()
{
	//设置触发器的碰撞检测
	//设置体积的状态-无碰撞
	LeftAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseEnemy::ActiveRightAttackCollision()
{
	//设置触发器的碰撞检测
	//设置体积的状态-查询与物理
	RightAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//指定当前的碰撞类型
	RightAttackCollision->SetCollisionObjectType(ECC_WorldDynamic);
	//只响应pawn，其他不响应
	RightAttackCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightAttackCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABaseEnemy::DeactiveRightAttackCollision()
{
	//设置触发器的碰撞检测
	//设置体积的状态-无碰撞
	RightAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

float ABaseEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	//减血
	if (Health - Damage <= 0.0f)
	{
		Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
		Die();
	}
	else
	{
		Health -= Damage;
	}

	//更新HealthBar
	HealthBar->SetPercent(Health / MaxHealth);

	return Health;
}

void ABaseEnemy::Die()
{
	//死亡状态
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Dead;

	//消除碰撞
	DeactiveLeftAttackCollision();
	DeactiveRightAttackCollision();
	AttackVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChaseVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//使玩家改变攻击目标
	((AMainPlayer*)UGameplayStatics::GetPlayerPawn(this, 0))->UpdateAttackTarget();
}

void ABaseEnemy::DeathEnd()
{
}

bool ABaseEnemy::HasValidTarget()
{
	return ((AMainPlayer*)UGameplayStatics::GetPlayerPawn(this, 0))->MovementStatus != EPlayerMovementStatus::EPMS_Dead;
}

