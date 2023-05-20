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

}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();

	//���ֲ����꣨meta=(MakeEditWidget="true")Ϊ�ֲ����꣩ת��Ϊ��������
	EndPoint += StartPoint;
	
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//��ȡ��ǰλ��
	const FVector CurrentLocation = GetActorLocation();
	//��ȡ��ֵ�ƶ����λ��(������)
	const FVector InterLocation= FMath::VInterpTo(CurrentLocation,EndPoint,DeltaTime,InterpSpeed);
	//��ȡ��ֵ�ƶ����λ��(����,��Ҫ���ٶ�ֵ��InterpSpeed������100��)
	//const FVector InterLocation = FMath::VInterpConstantTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
	//�������ƶ�����λ��
	SetActorLocation(InterLocation);
}

