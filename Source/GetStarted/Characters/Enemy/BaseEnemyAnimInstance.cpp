// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/BaseEnemyAnimInstance.h"

#include "BaseEnemy.h"

void UBaseEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EnemyRef = Cast<ABaseEnemy>(TryGetPawnOwner());
}

void UBaseEnemyAnimInstance::UpdateAnimationProperties()
{
	//���Ϊ�գ������¸�ֵ
	if (EnemyRef == nullptr)
	{
		EnemyRef = Cast<ABaseEnemy>(TryGetPawnOwner());
	}

	if (EnemyRef)
	{
		//��ȡ��ɫ���ٶ�
		const FVector SpeedVector = EnemyRef->GetVelocity();
		//ֻҪƽ����ٶȣ���ҪZ����ٶ�
		const FVector PlanarSpeed = FVector(SpeedVector.X, SpeedVector.Y, 0.0f);

		//ΪSpeed��ֵ
		Speed = PlanarSpeed.Size();

		//�����BlendSpace,��Ҫ��ȡ��ɫ����
		//float Direction = CalculateDirection(EnemyRef->GetVelocity(), EnemyRef->GetActorRotation());
	}
}
