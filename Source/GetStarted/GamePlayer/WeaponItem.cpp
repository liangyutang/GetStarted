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

	//删除父类中的组件
	DisplayMesh->DestroyComponent();
	//替换父类的组件,一定要在TEXT中重命名，防止引擎崩溃
	DisplayMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DisplaySkeletalMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());
	//激活碰撞
	ActiveDisplayMeshCollision();

	//硬编码加载音效
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

void AWeaponItem::Equip(AMainPlayer* MainPlayer)
{
	if (MainPlayer)
	{
		//修改武器状态
		WeaponState = EWeaponState::EWS_Equipped;

		//关闭碰撞
		DeactiveDisplayMeshCollision();

		//获取Socket
		const USkeletalMeshSocket* RightHandSocket = MainPlayer->GetMesh()->GetSocketByName("RightHandSocket");
		if (RightHandSocket)
		{
			//武器附着在socket上
			RightHandSocket->AttachActor(this, MainPlayer->GetMesh());

			//对MainPlayer做维护
			MainPlayer->bHasWeapon = true;
			MainPlayer->EquippedWeapon = this;
			MainPlayer->OverlappingWeapon = nullptr;

			//关闭旋转
			bNeedRotate = false;
			//播放音效
			if (OnEquippedSound)
			{
				UGameplayStatics::PlaySound2D(this, OnEquippedSound);
			}
			//是否保留粒子效果
			if (!bShouldReserveIdleParticle)
			{
				//不保留
				IdleParticleComponent->Deactivate();
			}
		}
	}
}

void AWeaponItem::UnEquip(AMainPlayer* MainPlayer)
{
	//卸载武器判断，不能在其他动作中卸载武器（奔跑除外）
	//跳跃时不可卸载武器,攻击时不可卸载武器
	if (MainPlayer
		&& !(MainPlayer->GetMovementComponent()->IsFalling())
		&& !(MainPlayer->bIsAttacking))
	{
		WeaponState = EWeaponState::EWS_CanPickup;

		//激活碰撞
		ActiveDisplayMeshCollision();

		//对MainPlayer做维护
		MainPlayer->bHasWeapon = false;
		MainPlayer->EquippedWeapon = nullptr;
		//判断角色周围是否还有其他武器
		if (MainPlayer->OverlappingWeapon==nullptr)
		{
			MainPlayer->OverlappingWeapon = this;
		}

		//脱离socket
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		//设置武器朝向与大小
		SetActorRotation(FRotator(0.0f));
		SetActorScale3D(FVector(1.0f));

		//旋转与粒子效果
		bNeedRotate = true;
		IdleParticleComponent->Activate();
	}
}

void AWeaponItem::ActiveDisplayMeshCollision()
{
	//设置触发器的碰撞检测
	//设置体积的状态-查询与物理
	DisplayMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//指定当前的碰撞类型
	DisplayMesh->SetCollisionObjectType(ECC_WorldStatic);
	//只响应pawn，其他不响应
	DisplayMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	DisplayMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void AWeaponItem::DeactiveDisplayMeshCollision()
{
	//设置触发器的碰撞检测
	//设置体积的状态-无碰撞
	DisplayMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
