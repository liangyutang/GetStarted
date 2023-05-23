// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerAnimInstance.h"

#include "MainPlayer.h"
#include "GameFramework/PawnMovementComponent.h"

void UMainPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerRef = Cast<AMainPlayer>(TryGetPawnOwner());
}

void UMainPlayerAnimInstance::UpdateAnimationProperties()
{
	//如果为空，则重新赋值
	if (PlayerRef==nullptr)
	{
		PlayerRef = Cast<AMainPlayer>(TryGetPawnOwner());
	}

	if (PlayerRef)
	{
		//获取角色的速度
		const FVector SpeedVector = PlayerRef->GetVelocity();
		//只要平面的速度，不要Z轴的速度
		const FVector PlanarSpeed = FVector(SpeedVector.X, SpeedVector.Y, 0.0f);

		//为Speed赋值
		Speed = PlanarSpeed.Size();

		//是否为空中
		bIsInAir = PlayerRef->GetMovementComponent()->IsFalling();
	}
}
