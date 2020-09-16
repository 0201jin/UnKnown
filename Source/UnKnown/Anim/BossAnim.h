// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnim.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateTired, const bool, Tired);

UCLASS()
class UNKNOWN_API UBossAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	void GetBossTired(bool Tired);

protected:
	UPROPERTY(BluePrintAssignable, Category = "Boss")
		FUpdateTired TiredUpdate;

};
