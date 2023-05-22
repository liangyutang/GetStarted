// Fill out your copyright notice in the Description page of Project Settings.


#include "Level1GameMode.h"
#include "UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ALevel1GameMode::BeginPlay()
{
	Super::BeginPlay();
	if (MainUIClass)
	{
		//��UI�󶨵����0����Ҫ��GameMode��ָ�����Ǹ�UI
		MainUI = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), MainUIClass);
		if (MainUI)
		{
			//��UI��ӵ�����ӿ�
			MainUI->AddToViewport();
		}
	}
}
