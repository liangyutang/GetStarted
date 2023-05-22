// Fill out your copyright notice in the Description page of Project Settings.


#include "IneractableItem.h"
#include "Components/SphereComponent.h"
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
}

// Called when the game starts or when spawned
void AIneractableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIneractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

