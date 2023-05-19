// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerableDoor.generated.h"

UCLASS()
class GETSTARTED_API ATriggerableDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerableDoor();


	//��
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* DoorMesh;

	//������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TriggerMesh;

	//��������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* TriggerBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
