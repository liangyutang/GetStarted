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

	/**������ﵲ������������֮�䣬��Ҫ�����ﴦ��*/
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	ChaseVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ChaseVolume"));
	ChaseVolume->SetupAttachment(GetRootComponent());
	ChaseVolume->InitSphereRadius(800.0f);
	//������ײ���
	//ָ����ǰ����ײ����
	ChaseVolume->SetCollisionObjectType(ECC_WorldDynamic);
	//ֻ��Ӧpawn����������Ӧ,��֪��Ϊʲô��AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);����Ч
	ChaseVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	ChaseVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	

	AttackVolume = CreateDefaultSubobject<USphereComponent>(TEXT("AttackVolume"));
	AttackVolume->SetupAttachment(GetRootComponent());
	AttackVolume->InitSphereRadius(100.0f);
	//������ײ���
	//ָ����ǰ����ײ����
	AttackVolume->SetCollisionObjectType(ECC_WorldDynamic);
	//ֻ��Ӧpawn����������Ӧ,��֪��Ϊʲô��AttackVolume->SetCollisionResponseToAllChannels(ECR_Ignore);����Ч
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
	//�������Ļ����ʾ
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	//��С
	HealthBarWidgetComponent->SetDrawSize(FVector2D(125.0f, 10.0f));

	//����AI
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//����AI״̬Ϊ����״̬
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

	//������ʼ���ص�ʱ
	LeftAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnLeftAttackCollisionOverlapBegin);
	//���ص��¼�����ʱ
	LeftAttackCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnLeftAttackCollisionOverlapEnd);

	//������ʼ���ص�ʱ
	RightAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnRightAttackCollisionOverlapBegin);
	//���ص��¼�����ʱ
	RightAttackCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnRightAttackCollisionOverlapEnd);

	//��ȡѪ������Ӧ�����
	HealthBar= Cast<UProgressBar>(HealthBarWidgetComponent->GetUserWidgetObject()->GetWidgetFromName("HealthBar"));
	//���ðٷֱ�
	HealthBar->SetPercent(Health / MaxHealth);
	//�տ�ʼ����
	HealthBar->SetVisibility(ESlateVisibility::Hidden);

	AIController = Cast<AAIController>(GetController());
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ʹ������������
	//���ڹ���״̬��
	if (bInterpToPlayer)
	{
		//�����ҶԵ��˵�	����(��ȡ0����ң�GameplayStatics::GetPlayerPawn(this,0))
		const FRotator LookAtYaw(0.0f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UGameplayStatics::GetPlayerPawn(this,0)->GetActorLocation()).Yaw, 0.0f);
		//�������
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
			//׷�����
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
			/**ֹͣ׷��*/
			EnemyMovementStatus = EEnemyMovementStatus::EEMS_Idle;

			HealthBar->SetVisibility(ESlateVisibility::Hidden);

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
	if (OtherActor)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			//������ҵĹ���Ŀ��
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
			//�Ƿ񹥻����
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
			//��������Ч��
			if (MainPlayer->HitParticles)
			{
				//���Ǹ�λ������
				const USkeletalMeshSocket* AttackSocket =GetMesh()->GetSocketByName("LeftAttackSocket");
				if (AttackSocket)
				{
					const FVector SocketLocation = AttackSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(this, MainPlayer->HitParticles, SocketLocation, FRotator(0.0f), true);
				}
			}

			//��������
			if (MainPlayer->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, MainPlayer->HitSound);
			}

			//ʩ���˺�
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
			//��������Ч��
			if (MainPlayer->HitParticles)
			{
				//���Ǹ�λ������
				const USkeletalMeshSocket* AttackSocket = GetMesh()->GetSocketByName("RightAttackSocket");
				if (AttackSocket)
				{
					const FVector SocketLocation = AttackSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(this, MainPlayer->HitParticles, SocketLocation, FRotator(0.0f), true);
				}
			}

			//��������
			if (MainPlayer->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, MainPlayer->HitSound);
			}

			//ʩ���˺�
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
	//���ô���������ײ���
	//���������״̬-��ѯ������
	LeftAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//ָ����ǰ����ײ����
	LeftAttackCollision->SetCollisionObjectType(ECC_WorldDynamic);
	//ֻ��Ӧpawn����������Ӧ
	LeftAttackCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftAttackCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABaseEnemy::DeactiveLeftAttackCollision()
{
	//���ô���������ײ���
	//���������״̬-����ײ
	LeftAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseEnemy::ActiveRightAttackCollision()
{
	//���ô���������ײ���
	//���������״̬-��ѯ������
	RightAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//ָ����ǰ����ײ����
	RightAttackCollision->SetCollisionObjectType(ECC_WorldDynamic);
	//ֻ��Ӧpawn����������Ӧ
	RightAttackCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightAttackCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABaseEnemy::DeactiveRightAttackCollision()
{
	//���ô���������ײ���
	//���������״̬-����ײ
	RightAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseEnemy::MoveToTarget(const AMainPlayer* TargetPlayer)
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

		//����·��
		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		//������·����Ⱦ����
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
	//�������Ƿ��ڹ���
	if (EnemyMovementStatus!=EEnemyMovementStatus::EEMS_Attacking)
	{
		EnemyMovementStatus = EEnemyMovementStatus::EEMS_Attacking;

		bInterpToPlayer = true;

		//��ȡ����ʵ��
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			//��ȡ�����������
			const float PlayRate = FMath::RandRange(0.9f, 1.1f);
			//��ȡ���section
			const FString SectionName = FString::FromInt(FMath::RandRange(1, 3));

			//���������ʲ����Ǹ���̫��
			AnimInstance->Montage_Play(AttackMontage, PlayRate);
			//������̫����Ǹ�section
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
	//��Ѫ
	if (Health - Damage <= 0.0f)
	{
		Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
		Die();
	}
	else
	{
		Health -= Damage;
	}

	//����HealthBar
	HealthBar->SetPercent(Health / MaxHealth);

	return Health;
}

void ABaseEnemy::Die()
{
	//����״̬
	EnemyMovementStatus = EEnemyMovementStatus::EEMS_Dead;

	//������ײ
	DeactiveLeftAttackCollision();
	DeactiveRightAttackCollision();
	AttackVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChaseVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//ʹ��Ҹı乥��Ŀ��
	((AMainPlayer*)UGameplayStatics::GetPlayerPawn(this, 0))->UpdateAttackTarget();
}

void ABaseEnemy::DeathEnd()
{
}

bool ABaseEnemy::HasValidTarget()
{
	return ((AMainPlayer*)UGameplayStatics::GetPlayerPawn(this, 0))->MovementStatus != EPlayerMovementStatus::EPMS_Dead;
}

