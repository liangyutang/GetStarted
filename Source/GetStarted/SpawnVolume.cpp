// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	//获取源点
	const FVector Origin = SpawnBox->GetComponentLocation();
	//获取盒子大小(边界信息-不带缩放)
	const FVector Extent = SpawnBox->GetScaledBoxExtent();
	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActorClass()
{
	if (SpwanActorClassesArray.Num()>0)
	{
		//随机获取一个子类
		const int index = FMath::RandRange(0, SpwanActorClassesArray.Num() - 1);
		return SpwanActorClassesArray[index];
	}else
	{
		return nullptr;
	}
}

//c++默认实现
void ASpawnVolume::SpawnActor_Implementation(UClass* SpwanClass, FVector SpawnLocation)
{
	if (SpwanClass)
	{
		//SpawnActor<AActor>：生成一个AActor类型的指针，AActor具体类型看UClass(SpawnClass)
		GetWorld()->SpawnActor<AActor>(SpwanClass, SpawnLocation, FRotator(0.0f));
	}
}

