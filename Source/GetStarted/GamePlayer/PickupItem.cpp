// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer/PickupItem.h"

#include "MainPlayer.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

APickupItem::APickupItem()
{
	TriggerVolume->SetSphereRadius(50.0f);
}

void APickupItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//�����Ľ�ɫ�Ƿ��ڷ�Χ�ڣ��ڷ�Χ�ڲ����˺�
	if (OtherActor)
	{
		//����Trigger�������ж�,�Ƿ�Ϊ���
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
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
			OnPickup(MainPlayer);
			Destroy();
		}
	}
}

void APickupItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
