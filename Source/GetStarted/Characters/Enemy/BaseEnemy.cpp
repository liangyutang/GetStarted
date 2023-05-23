// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

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
	

	AttackVolume = CreateDefaultSubobject<USphereComponent>(TEXT("AttackVolume"));
	AttackVolume->SetupAttachment(GetRootComponent());
	AttackVolume->InitSphereRadius(100.0f);
	
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
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::OnChaseVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseEnemy::OnChaseVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseEnemy::OnAttackVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseEnemy::OnAttackVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

