// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer/WeaponItem.h"

#include "Components/SphereComponent.h"
#include "MainPlayer.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"

AWeaponItem::AWeaponItem()
{

	//ɾ�������е����
	DisplayMesh->DestroyComponent();
	//�滻��������,һ��Ҫ��TEXT������������ֹ�������
	DisplayMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DisplaySkeletalMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());
	//������ײ
	ActiveDisplayMeshCollision();

	//Ӳ���������Ч
	static  ConstructorHelpers::FObjectFinder<USoundCue> SoundCueAsset(TEXT("SoundCue'/Game/Assets/Audios/Blade_Cue.Blade_Cue'"));
	if (SoundCueAsset.Succeeded())
	{
		OnEquippedSound = SoundCueAsset.Object;
	}

	TriggerVolume->SetSphereRadius(64.0f);

	bShouldReserveIdleParticle = false;

	WeaponState = EWeaponState::EWS_CanPickup;

}

void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//��Χ���Ƿ���ʵ�壬����Ϊ��ʰȡ״̬
	if (OtherActor && WeaponState==EWeaponState::EWS_CanPickup)
	{
		//�Ƿ�Ϊ���
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			//Ϊ���OverlappingWeapon���Ը�ֵ
			MainPlayer->OverlappingWeapon = this;
		}
	}
}

void AWeaponItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	//��Χ���Ƿ���ʵ�壬����Ϊ��ʰȡ״̬
	if (OtherActor)
	{
		//�Ƿ�Ϊ��ң�����Ƿ�ӱ�������Χ�뿪
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer && MainPlayer->OverlappingWeapon==this)
		{
			//���OverlappingWeapon�����ÿ�
			MainPlayer->OverlappingWeapon = nullptr;
		}
	}
}

void AWeaponItem::Equip(AMainPlayer* MainPlayer)
{
	if (MainPlayer)
	{
		//�޸�����״̬
		WeaponState = EWeaponState::EWS_Equipped;

		//�ر���ײ
		DeactiveDisplayMeshCollision();

		//��ȡSocket
		const USkeletalMeshSocket* RightHandSocket = MainPlayer->GetMesh()->GetSocketByName("RightHandSocket");
		if (RightHandSocket)
		{
			//����������socket��
			RightHandSocket->AttachActor(this, MainPlayer->GetMesh());

			//��MainPlayer��ά��
			MainPlayer->bHasWeapon = true;
			MainPlayer->EquippedWeapon = this;
			MainPlayer->OverlappingWeapon = nullptr;

			//�ر���ת
			bNeedRotate = false;
			//������Ч
			if (OnEquippedSound)
			{
				UGameplayStatics::PlaySound2D(this, OnEquippedSound);
			}
			//�Ƿ�������Ч��
			if (!bShouldReserveIdleParticle)
			{
				//������
				IdleParticleComponent->Deactivate();
			}
		}
	}
}

void AWeaponItem::UnEquip(AMainPlayer* MainPlayer)
{
	//ж�������жϣ�����������������ж�����������ܳ��⣩
	//��Ծʱ����ж������,����ʱ����ж������
	if (MainPlayer
		&& !(MainPlayer->GetMovementComponent()->IsFalling())
		&& !(MainPlayer->bIsAttacking))
	{
		WeaponState = EWeaponState::EWS_CanPickup;

		//������ײ
		ActiveDisplayMeshCollision();

		//��MainPlayer��ά��
		MainPlayer->bHasWeapon = false;
		MainPlayer->EquippedWeapon = nullptr;
		//�жϽ�ɫ��Χ�Ƿ�����������
		if (MainPlayer->OverlappingWeapon==nullptr)
		{
			MainPlayer->OverlappingWeapon = this;
		}

		//����socket
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		//���������������С
		SetActorRotation(FRotator(0.0f));
		SetActorScale3D(FVector(1.0f));

		//��ת������Ч��
		bNeedRotate = true;
		IdleParticleComponent->Activate();
	}
}

void AWeaponItem::ActiveDisplayMeshCollision()
{
	//���ô���������ײ���
	//���������״̬-��ѯ������
	DisplayMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//ָ����ǰ����ײ����
	DisplayMesh->SetCollisionObjectType(ECC_WorldStatic);
	//ֻ��Ӧpawn����������Ӧ
	DisplayMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	DisplayMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void AWeaponItem::DeactiveDisplayMeshCollision()
{
	//���ô���������ײ���
	//���������״̬-����ײ
	DisplayMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
