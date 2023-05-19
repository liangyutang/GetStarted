// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class GETSTARTED_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UBoxComponent* SpawnBox;

	//Ҫ�������������
	//����һ��AActor���ģ������,���м̳�AActor�����඼���Է���TSubclassOf
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Spawn Volume")
	TArray<TSubclassOf<AActor>> SpwanActorClassesArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//��ȡ���һ����
	UFUNCTION(BlueprintCallable, Category = "Spawn Volume")
	FVector GetSpawnPoint();

	//��ȡ���AActor��һ������
	UFUNCTION(BlueprintCallable, Category = "Spawn Volume")
	TSubclassOf<AActor> GetSpawnActorClass();
};
