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

	//meta=(MakeEditWidget="true")在世界大纲中可以为改变量创建一个坐标轴，移动改坐标轴不会改变物体的位置，为设计人员提供定位方便
	UPROPERTY(EditAnywhere, Category = "Floating Platform",meta=(MakeEditWidget="true"))
	FVector EndPoint;


	//物体移动速度
	UPROPERTY(EditAnywhere, Category = "Floating Platform")
	float InterpSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
