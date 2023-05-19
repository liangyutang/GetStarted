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

	//要生成那类的物体
	//创建一个AActor类的模板数组,所有继承AActor的子类都可以放入TSubclassOf
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Spawn Volume")
	TArray<TSubclassOf<AActor>> SpwanActorClassesArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//获取随机一个点
	UFUNCTION(BlueprintCallable, Category = "Spawn Volume")
	FVector GetSpawnPoint();

	//获取随机AActor的一个子类
	UFUNCTION(BlueprintCallable, Category = "Spawn Volume")
	TSubclassOf<AActor> GetSpawnActorClass();
};
