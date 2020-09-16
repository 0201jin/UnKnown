// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Player/Hero.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWN_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void GetInfo();
};
