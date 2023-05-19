// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerableDoor.h"

#include "Components/BoxComponent.h"
#include "MainPlayer.h"

// Sets default values
ATriggerableDoor::ATriggerableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	//���ô�������С
	TriggerBox->SetBoxExtent(FVector(60.0f, 60.0f, 30.0f));

	//���ô���������ײ���
	//���������״̬-����ѯ
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//ָ����ǰ����ײ����
	TriggerBox->SetCollisionObjectType(ECC_WorldStatic);
	//ֻ��Ӧpawn����������Ӧ
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);


	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(GetRootComponent());

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ATriggerableDoor::BeginPlay()
{
	Super::BeginPlay();
	//������ʼ���ص�ʱ
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this,&ATriggerableDoor::OnOverlapBegin);

	//���ص��¼�����ʱ
	TriggerBox->OnComponentEndOverlap.AddDynamic(this,&ATriggerableDoor::OnOverlapEnd);
	
}

// Called every frame
void ATriggerableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerableDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//����Trigger�������ж�,�Ƿ�Ϊ���
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
	if (MainPlayer)
	{
		//����
		OpenDoor();
		//����̨��
		LowerTrigger();
	}
}

void ATriggerableDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//����Trigger�������ж�,�Ƿ�Ϊ���
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
	if (MainPlayer)
	{
		//����̨��
		RaiseTrigger();
		//����
		CloseDoor();
	}
}

