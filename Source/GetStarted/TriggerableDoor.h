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


	//门
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* DoorMesh;

	//玩家看到的触发器
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TriggerMesh;

	//触发盒子
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* TriggerBox;

	UPROPERTY(BlueprintReadWrite,Category="Triggerable Door|Trigger Properties")
	FVector InitTriggerLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Triggerable Door|Door Properties")
	FVector InitDoorLocation;

	//延迟时间
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Triggerable Door|Door Properties")
	float DelayTime;

	//触发器关闭时的定时器
	FTimerHandle CloseDoorTimeHandle;

	bool bIsPlayerOnTrigger;

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

	UFUNCTION(BlueprintImplementableEvent,Category="Triggerable Door|Trigger Switch")
	void RaiseTrigger();

	UFUNCTION(BlueprintImplementableEvent, Category = "Triggerable Door|Trigger Switch")
	void LowerTrigger();

	UFUNCTION(BlueprintImplementableEvent, Category = "Triggerable Door|Door Switch")
	void OpenDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Triggerable Door|Door Switch")
	void CloseDoor();

	UFUNCTION(BlueprintCallable, Category = "Triggerable Door|Trigger Switch")
	void UpdateTriggerLocation(FVector Offset);

	UFUNCTION(BlueprintCallable, Category = "Triggerable Door|Door Switch")
	void UpdateDoorLocation(FVector Offset);
};
