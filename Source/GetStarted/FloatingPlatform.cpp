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
	//���ֲ����꣨meta=(MakeEditWidget="true")Ϊ�ֲ����꣩ת��Ϊ��������
	EndPoint += StartPoint;

	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		//��ȡ��ǰλ��
		const FVector CurrentLocation = GetActorLocation();
		//��ȡ��ֵ�ƶ����λ��(������)
		const FVector InterLocation = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		//��ȡ��ֵ�ƶ����λ��(����,��Ҫ���ٶ�ֵ��InterpSpeed������100��)
		//const FVector InterLocation = FMath::VInterpConstantTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		//�������ƶ�����λ��
		SetActorLocation(InterLocation);

		//�������
		float NowDistance = (GetActorLocation() - StartPoint).Size();
		//�ж�����float���͵�λ�Ƿ���ȣ���������ֵ�Ƿ�С��һ����С�������ж�
		if (Distance-NowDistance<=0.5f)
		{
			//���ƶ���EndPoint��ֹͣ�ƶ�����StartPoint��EndPoint�Ի���ͣ��2������½����ƶ�

			const auto ToggleInterpState=[this]()
			{
				bInterping = !bInterping;
			};

			//ֹͣ�ƶ�
			ToggleInterpState();

			//������ʱ��,2���ִ�������lambda���ʽ������������ƶ�
			GetWorldTimerManager().SetTimer(InterpTimerHandle, FTimerDelegate::CreateLambda(ToggleInterpState), DelayTime, false);

			//��StartPoint��EndPoint�Ի�
			const FVector TempVector = StartPoint;
			StartPoint = EndPoint;
			EndPoint = TempVector;
		}
	}

	
}

