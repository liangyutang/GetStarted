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

	//设置触发器大小
	TriggerBox->SetBoxExtent(FVector(60.0f, 60.0f, 30.0f));

	//设置触发器的碰撞检测
	//设置体积的状态-仅查询
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//指定当前的碰撞类型
	TriggerBox->SetCollisionObjectType(ECC_WorldStatic);
	//只响应pawn，其他不响应
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
	//当区域开始被重叠时
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this,&ATriggerableDoor::OnOverlapBegin);

	//当重叠事件结束时
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
	//触碰Trigger的物体判断,是否为玩家
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
	if (MainPlayer)
	{
		//开门
		OpenDoor();
		//降低台子
		LowerTrigger();
	}
}

void ATriggerableDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//触碰Trigger的物体判断,是否为玩家
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
	if (MainPlayer)
	{
		//升高台子
		RaiseTrigger();
		//关门
		CloseDoor();
	}
}

