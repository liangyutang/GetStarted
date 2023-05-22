// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GetStartedGameModeBase.h"
#include "Level1GameMode.generated.h"

/**
 * 
 */
UCLASS()
class GETSTARTED_API ALevel1GameMode : public AGetStartedGameModeBase
{
	GENERATED_BODY()


public:
	//´´½¨UI
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI Widgets")
	TSubclassOf<UUserWidget> MainUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI Widgets")
	UUserWidget * MainUI;


protected:
	virtual void BeginPlay() override;
};
