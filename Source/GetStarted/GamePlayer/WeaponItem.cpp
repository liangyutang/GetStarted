// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer/WeaponItem.h"

#include "Components/SphereComponent.h"

AWeaponItem::AWeaponItem()
{

	//删除父类中的组件
	DisplayMesh->DestroyComponent();
	//替换父类的组件,一定要在TEXT中重命名，防止引擎崩溃
	DisplayMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DisplaySkeletalMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());

	TriggerVolume->SetSphereRadius(64.0f);
}
