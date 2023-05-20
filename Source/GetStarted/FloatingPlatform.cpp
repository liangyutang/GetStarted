// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;

	StartPoint = FVector(0.0f);
	EndPoint = FVector(0.0f);

	InterpSpeed = 2.0f;

	DelayTime = 2.0f;
	bInterping = true;

}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();
	//将局部坐标（meta=(MakeEditWidget="true")为局部坐标）转换为世界坐标
	EndPoint += StartPoint;

	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		//获取当前位置
		const FVector CurrentLocation = GetActorLocation();
		//获取插值移动后的位置(不匀速)
		const FVector InterLocation = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		//获取插值移动后的位置(匀速,需要将速度值（InterpSpeed）调大100倍)
		//const FVector InterLocation = FMath::VInterpConstantTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		//将物体移动到该位置
		SetActorLocation(InterLocation);

		//计算距离
		float NowDistance = (GetActorLocation() - StartPoint).Size();
		//判断两个float类型单位是否相等，最好用相减值是否小于一个较小的数来判断
		if (Distance-NowDistance<=0.5f)
		{
			//若移动到EndPoint，停止移动，将StartPoint与EndPoint对换，停留2秒后，重新进行移动

			const auto ToggleInterpState=[this]()
			{
				bInterping = !bInterping;
			};

			//停止移动
			ToggleInterpState();

			//开启定时器,2秒后，执行上面的lambda表达式，让物体可以移动
			GetWorldTimerManager().SetTimer(InterpTimerHandle, FTimerDelegate::CreateLambda(ToggleInterpState), DelayTime, false);

			//将StartPoint与EndPoint对换
			const FVector TempVector = StartPoint;
			StartPoint = EndPoint;
			EndPoint = TempVector;
		}
	}

	
}

