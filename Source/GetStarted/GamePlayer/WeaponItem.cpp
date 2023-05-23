// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer/WeaponItem.h"

#include "Components/SphereComponent.h"

AWeaponItem::AWeaponItem()
{

	//ɾ�������е����
	DisplayMesh->DestroyComponent();
	//�滻��������,һ��Ҫ��TEXT������������ֹ�������
	DisplayMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DisplaySkeletalMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());

	TriggerVolume->SetSphereRadius(64.0f);
}
