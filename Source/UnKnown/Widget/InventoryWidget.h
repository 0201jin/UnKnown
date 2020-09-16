// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget.h"
#include "Player/Hero.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */

UCLASS()
class UNKNOWN_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void SendInvenInfo(int _count, UObject * _Image, UObject * _Slot, FString Name, FString Explain, bool _Consumable, int _Parts, int _Code);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void SendToolTipInfo(FString _Name, FString _Explain, int _Slot);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void HealHero(int _Heal);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void SetActive1(FString _Name);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void SetActive2(FString _Name);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void InitActive(FString _Name);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void ChangeItem(int _NewItemCode);
};
