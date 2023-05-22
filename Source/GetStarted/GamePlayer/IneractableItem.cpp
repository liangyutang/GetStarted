// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer/IneractableItem.h"
#include <Components/SphereComponent.h>

#include "Particles/ParticleSystemComponent.h"

// Sets default values
AIneractableItem::AIneractableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetupAttachment(GetRootComponent());

	IdleParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent"));
	IdleParticleComponent->SetupAttachment(GetRootComponent());

	bNeedRotate = true;

	RotationRate = 45.0f;

}

// Called when the game starts or when spawned
void AIneractableItem::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AIneractableItem::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AIneractableItem::OnOverlapEnd);
	
}

// Called every frame
void AIneractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ÉèÖÃÐý×ª
	if (bNeedRotate)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += RotationRate * DeltaTime;
		SetActorRotation(NewRotation);
	}
}

void AIneractableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AIneractableItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

