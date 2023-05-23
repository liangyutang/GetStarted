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
	//���Ϊ�գ������¸�ֵ
	if (PlayerRef==nullptr)
	{
		PlayerRef = Cast<AMainPlayer>(TryGetPawnOwner());
	}

	if (PlayerRef)
	{
		//��ȡ��ɫ���ٶ�
		const FVector SpeedVector = PlayerRef->GetVelocity();
		//ֻҪƽ����ٶȣ���ҪZ����ٶ�
		const FVector PlanarSpeed = FVector(SpeedVector.X, SpeedVector.Y, 0.0f);

		//ΪSpeed��ֵ
		Speed = PlanarSpeed.Size();

		//�Ƿ�Ϊ����
		bIsInAir = PlayerRef->GetMovementComponent()->IsFalling();
	}
}
