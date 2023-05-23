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

void AWeaponItem::BeginPlay()
{
}

void AWeaponItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AWeaponItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AWeaponItem::Equip()
{
}

void AWeaponItem::UnEquip()
{
}
