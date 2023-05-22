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

	//触碰的角色是否还在范围内，在范围内才有伤害
	if (OtherActor)
	{
		//触碰Trigger的物体判断,是否为玩家
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
		if (MainPlayer)
		{
			//是否播放粒子系统
			if (OverlapParticle)
			{
				//播放粒子效果
				//在指定位置生成粒子发射器（SpawnEmitterAtLocation（谁生成，生成那个粒子效果，那个位置生成，旋转速率，是否自动销毁））
				UGameplayStatics::SpawnEmitterAtLocation(this, OverlapParticle, GetActorLocation(), FRotator(0.0f), true);
			}

			//是否播放声音
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
