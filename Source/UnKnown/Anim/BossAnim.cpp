// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnim.h"

void UBossAnim::GetBossTired(bool Tired)
{
	TiredUpdate.Broadcast(Tired);
}