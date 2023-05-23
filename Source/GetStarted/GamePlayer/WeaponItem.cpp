// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer/WeaponItem.h"

#include "Components/SphereComponent.h"
#include "MainPlayer.h"

AWeaponItem::AWeaponItem()
{

	//ɾ�������е����
	DisplayMesh->DestroyComponent();
	//�滻��������,һ��Ҫ��TEXT������������ֹ�������
	DisplayMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DisplaySkeletalMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());

	TriggerVolume->SetSphereRadius(64.0f);

	bShouldReserveIdleParticle = false;

	WeaponState = EWeaponState::EWS_CanPickup;
}

void AWeaponItem::BeginPlay()
{
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

void AWeaponItem::Equip()
{
}

void AWeaponItem::UnEquip()
{
}
