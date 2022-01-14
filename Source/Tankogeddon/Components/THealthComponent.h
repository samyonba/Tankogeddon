// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tankogeddon/Interfaces/IDamageTaker.h"
#include "THealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TANKOGEDDON_API UTHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
	DECLARE_MULTICAST_DELEGATE(FOnDieDelegate)

public:
	FOnDieDelegate OnDieDelegate;
	
public:
	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY()
	float CurrentHealth;

public:
	UTHealthComponent();
	void SetMaxHealth(float Value);
	void TakeDamage(const FTDamageData& DamageData);
	float GetHealth() const;
	float GetHealthState() const;
	void AddHealth(float AdditionalHealth);
	
};
