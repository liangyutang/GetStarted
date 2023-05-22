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
	//Componentһ�㲻���ࣨCategory��

	//����ϵͳ��2�֣�����ǰ������ϵͳ�������������ϵͳ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* IdleParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Item|Particles")
	UParticleSystem* OverlapParticle;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Item|Sound")
	class USoundCue* OverlapSound;

	//�Ƿ���ת
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Item|Item Properties")
	bool bNeedRotate;

	//��ת�ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Item|Item Properties")
	float RotationRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex);


};
