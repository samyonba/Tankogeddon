// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageTaker.generated.h"

USTRUCT()
struct FTDamageData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	float DamageValue;
	UPROPERTY()
	AActor* Instigator;
	UPROPERTY()
	AActor* Causer;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDamageTaker : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKOGEDDON_API IIDamageTaker
{
	GENERATED_BODY()

public:
	virtual void TakeDamage(const FTDamageData& DamageData) = 0;
};
