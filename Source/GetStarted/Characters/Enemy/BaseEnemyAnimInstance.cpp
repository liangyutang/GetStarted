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
	//如果为空，则重新赋值
	if (EnemyRef == nullptr)
	{
		EnemyRef = Cast<ABaseEnemy>(TryGetPawnOwner());
	}

	if (EnemyRef)
	{
		//获取角色的速度
		const FVector SpeedVector = EnemyRef->GetVelocity();
		//只要平面的速度，不要Z轴的速度
		const FVector PlanarSpeed = FVector(SpeedVector.X, SpeedVector.Y, 0.0f);

		//为Speed赋值
		Speed = PlanarSpeed.Size();

		//如果的BlendSpace,需要获取角色朝向
		//float Direction = CalculateDirection(EnemyRef->GetVelocity(), EnemyRef->GetActorRotation());
	}
}
