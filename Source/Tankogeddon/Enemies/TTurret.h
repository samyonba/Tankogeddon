// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tankogeddon/Core/TPawn.h"
#include "Tankogeddon/Interfaces/IDamageTaker.h"
#include "TTurret.generated.h"

UCLASS()
class TANKOGEDDON_API ATTurret : public ATPawn
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	APawn* PlayerPawn;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATCannon> CannonClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TargetingRange = 1000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TargetingSpeed = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TargetingRate = 0.005f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FireAccuracy = 10.f;

public:
	ATTurret();
	virtual void TakeDamage(const FTDamageData& DamageData) override;

protected:
	virtual void BeginPlay() override;
	AActor* GetTarget() const;
	void Targeting();
	void RotateToTarget();
	bool IsTargetInRange() const;
	bool CanFire() const;
	
	virtual void OnDie() override;
	
};
