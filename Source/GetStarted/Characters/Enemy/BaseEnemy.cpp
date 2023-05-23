// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseEnemy.h"

#include "AIController.h"
#include "MainPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/**������ﵲ������������֮�䣬��Ҫ�����ﴦ��*/
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	ChaseVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ChaseVolume"));
	ChaseVolume->SetupAttachment(GetRootComponent());
	ChaseVolume->InitSphereRadius(800.0f);
	

	AttackVolume = CreateDefaultSubobject<USphereComponent>(TEXT("AttackVolume"));
	AttackVolume->SetupAttachment(GetRootComponent());
	AttackVolume->InitSphereRadius(100.0f);

	//����AI
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//����AI״̬Ϊ����״̬
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	//������ײ���
	//ָ����ǰ����ײ����
	ChaseVolume->SetCollisionObjectType(ECC_WorldDynamic);
	//ֻ��Ӧpawn����������Ӧ,��֪��Ϊʲô��AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);����Ч
	ChaseVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	ChaseVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//������ײ���
	//ָ����ǰ����ײ����
	AttackVolume->SetCollisionObjectType(ECC_WorldDynamic);
	//ֻ��Ӧpawn����������Ӧ,��֪��Ϊʲô��AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);����Ч
	AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//������ʼ���ص�ʱ
	ChaseVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnChaseVolumeOverlapBegin);
	//���ص��¼�����ʱ
	ChaseVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnChaseVolumeOverlapEnd);

	//������ʼ���ص�ʱ
	AttackVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnAttackVolumeOverlapBegin);
	//���ص��¼�����ʱ
	AttackVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnAttackVolumeOverlapEnd);

	AIController = Cast<AAIController>(GetController());
	
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
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			//׷�����
			MoveToTarget(MainPlayer);
		}
	}
}

void ABaseEnemy::OnChaseVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			/**ֹͣ׷��*/
			EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;

			if (AIController)
			{
				//ֹͣ�ƶ�
				AIController->StopMovement();
			}
		}
	}
}

void ABaseEnemy::OnAttackVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseEnemy::OnAttackVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseEnemy::MoveToTarget(AMainPlayer* TargetPlayer)
{
	//����Ϊ׷��״̬
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_MoveToTarget;

	if (AIController)
	{
		//׼������
		FAIMoveRequest MoveRequest;
		//����׷��Ŀ��
		MoveRequest.SetGoalActor(TargetPlayer);
		//������˶������׷��
		MoveRequest.SetAcceptanceRadius(10.0f);

		//Ŀ���λ��
		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}
}

