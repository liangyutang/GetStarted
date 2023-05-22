// Fill out your copyright notice in the Description page of Project Settings.


#include "Level1GameMode.h"
#include "UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ALevel1GameMode::BeginPlay()
{
	Super::BeginPlay();
	if (MainUIClass)
	{
		//将UI绑定到玩家0，需要在GameMode中指定是那个UI
		MainUI = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), MainUIClass);
		if (MainUI)
		{
			//将UI添加到玩家视口
			MainUI->AddToViewport();
		}
	}
}
