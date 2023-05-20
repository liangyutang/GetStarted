// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class GETSTARTED_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(VisibleAnywhere, Category="Floating Platform")
	FVector StartPoint;

	//meta=(MakeEditWidget="true")���������п���Ϊ�ı�������һ�������ᣬ�ƶ��������᲻��ı������λ�ã�Ϊ�����Ա�ṩ��λ����
	UPROPERTY(EditAnywhere, Category = "Floating Platform",meta=(MakeEditWidget="true"))
	FVector EndPoint;


	//�����ƶ��ٶ�
	UPROPERTY(EditAnywhere, Category = "Floating Platform")
	float InterpSpeed;

	//�ӳ�ʱ��
	UPROPERTY(EditAnywhere, Category = "Floating Platform")
	float DelayTime;

	//�Ƿ����˶��У��Ƿ������ӳ٣�
	bool bInterping;

	//��ʱ���ľ��
	FTimerHandle InterpTimerHandle;

	//��ʼ������ľ���
	float Distance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
