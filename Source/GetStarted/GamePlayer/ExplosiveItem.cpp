// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer/ExplosiveItem.h"

#include "MainPlayer.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Characters/Enemy/BaseEnemy.h"

AExplosiveItem::AExplosiveItem()
{
	Damage = 20.0f;

	TriggerVolume->SetSphereRadius(50.0f);
}

void AExplosiveItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//�����Ľ�ɫ�Ƿ��ڷ�Χ�ڣ��ڷ�Χ�ڲ����˺�
	if (OtherActor)
	{
		//����Trigger�������ж�,�Ƿ�Ϊ���
		const AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		const ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(OtherActor);
		if (MainPlayer || BaseEnemy)
		{
			//�Ƿ񲥷�����ϵͳ
			if (OverlapParticle)
			{
				//��������Ч��
				//��ָ��λ���������ӷ�������SpawnEmitterAtLocation��˭���ɣ������Ǹ�����Ч�����Ǹ�λ�����ɣ���ת���ʣ��Ƿ��Զ����٣���
				UGameplayStatics::SpawnEmitterAtLocation(this, OverlapParticle, GetActorLocation(), FRotator(0.0f), true);
			}

			//�Ƿ񲥷�����
			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}
			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);
			Destroy();
		}
	}
}

void AExplosiveItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
