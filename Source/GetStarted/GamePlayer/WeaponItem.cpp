// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer/WeaponItem.h"

#include "Components/SphereComponent.h"
#include "MainPlayer.h"

AWeaponItem::AWeaponItem()
{

	//删除父类中的组件
	DisplayMesh->DestroyComponent();
	//替换父类的组件,一定要在TEXT中重命名，防止引擎崩溃
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

	//范围内是否有实体，并且为可拾取状态
	if (OtherActor && WeaponState==EWeaponState::EWS_CanPickup)
	{
		//是否为玩家
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			//为玩家OverlappingWeapon属性赋值
			MainPlayer->OverlappingWeapon = this;
		}
	}
}

void AWeaponItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	//范围内是否有实体，并且为可拾取状态
	if (OtherActor)
	{
		//是否为玩家，玩家是否从本武器范围离开
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer && MainPlayer->OverlappingWeapon==this)
		{
			//玩家OverlappingWeapon属性置空
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
