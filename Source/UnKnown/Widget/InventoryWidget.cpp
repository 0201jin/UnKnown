// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"

void UInventoryWidget::SendInvenInfo(int _count, UObject * _Image, UObject * _Slot, FString _Name, FString _Explain, bool _Consumable, int _Parts, int _Code)
{
	AHero * hero = Cast<AHero>(GetOwningPlayerPawn());
	hero->InvenUpdate(_count, _Image, _Slot, _Name, _Explain, _Consumable, _Parts, _Code);
}

void UInventoryWidget::SendToolTipInfo(FString _Name, FString _Explain, int _Slot)
{
	AHero * hero = Cast<AHero>(GetOwningPlayerPawn());
	hero->SendToolTipInfo(_Name, _Explain, _Slot);
}

void UInventoryWidget::HealHero(int _Heal)
{
	AHero * hero = Cast<AHero>(GetOwningPlayerPawn());
	hero->HealHero(_Heal);
}

void UInventoryWidget::SetActive1(FString _Name)
{
	AHero * hero = Cast<AHero>(GetOwningPlayerPawn());
	hero->SetActive1(_Name);
}

void UInventoryWidget::SetActive2(FString _Name)
{
	AHero * hero = Cast<AHero>(GetOwningPlayerPawn());
	hero->SetActive2(_Name);
}

void UInventoryWidget::InitActive(FString _Name)
{
	AHero * hero = Cast<AHero>(GetOwningPlayerPawn());
	hero->InitActive(_Name);
}

void UInventoryWidget::ChangeItem(int _NewItemCode)
{
	AHero * hero = Cast<AHero>(GetOwningPlayerPawn());
	hero->ChangeItem(_NewItemCode);
}
