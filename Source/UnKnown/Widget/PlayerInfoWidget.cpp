// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInfoWidget.h"

void UPlayerInfoWidget::GetInfo()
{
	AHero * hero = Cast<AHero>(GetOwningPlayerPawn());
	hero->GetPlayerInfo();
}