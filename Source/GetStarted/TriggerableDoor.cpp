// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerableDoor.h"

#include "Components/BoxComponent.h"

// Sets default values
ATriggerableDoor::ATriggerableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

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
}

void ATriggerableDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

