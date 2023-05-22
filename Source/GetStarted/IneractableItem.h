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

	//���δ�����
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USphereComponent* TriggerVolume;

	//��ʾģ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DisplayMesh;

	//�����Component������Ҫ��cpp�ļ��д����������CreateDefaultSubobject����������Component����ɿ�Ϊ��Դ�ļ������������ͳһ����

	//����ϵͳ��2�֣�����ǰ������ϵͳ�������������ϵͳ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Interactable Item|Particles")
	UParticleSystemComponent* IdleParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Item|Particles")
	UParticleSystem* OverlapParticle;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Item|Sound")
	class USoundCue* OverlapSound;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
