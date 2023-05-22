// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IneractableItem.generated.h"

UCLASS()
class GETSTARTED_API AIneractableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIneractableItem();

	//球形触发器
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USphereComponent* TriggerVolume;

	//显示模型
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DisplayMesh;

	//如果带Component，则需要在cpp文件中创建改组件（CreateDefaultSubobject），若不带Component，则可看为资源文件，有虚幻引擎统一管理

	//粒子系统，2种，交换前的粒子系统，交互后的粒子系统
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Interactable Item|Particles")
	UParticleSystemComponent* IdleParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Item|Particles")
	UParticleSystem* OverlapParticle;

	//音乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Item|Sound")
	class USoundCue* OverlapSound;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
