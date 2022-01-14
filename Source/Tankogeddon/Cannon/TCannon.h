// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Tankogeddon/Projectiles/TProjectile.h"
#include "TCannon.generated.h"

class UStaticMeshComponent;

UCLASS()
class TANKOGEDDON_API ATCannon : public AActor
{
	GENERATED_BODY()

public:
	ATCannon();
	bool IsReadyToFire();
	virtual void Fire();
	virtual void StartFireSpecial();
	virtual void StopFireSpecial();
	
	int GetFireAmmunition() const;
	int GetSpecialFireAmmunition() const;
	
	void ReloadCannon();
	void IncreaseFireAmmunition(int value);
	void IncreaseSpecialFireAmmunition(int value);

protected:
	virtual void DoFireSpecial();
	void SetReadyToFire();
	void SetNotReadyToFire();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* TransformComponent;
	
	// UPROPERTY(VisibleAnywhere)
	// UStaticMeshComponent* CannonMesh;

	UPROPERTY(EditAnywhere)
	UArrowComponent* FirePoint;

	UPROPERTY(EditAnywhere)
	int FireAmmunition = 30;

	UPROPERTY(EditAnywhere)
	int FireSpecialAmmunition = 500;

	UPROPERTY(EditAnywhere)
	float ReloadTime = 1.f;

	UPROPERTY(EditAnywhere)
	float FireSpecialReloadTime = 0.15f;

	UPROPERTY(EditAnywhere)
	float FireDamage = 1.f;

	UPROPERTY(EditAnywhere)
	float FireSpecialDamage = 0.1f;

	UPROPERTY(EditAnywhere)
	float TraceFireRange = 1000.f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditAnywhere)
	bool bDrawDebugTrace = true;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATProjectile> ProjectileClass;
	
	FTimerHandle ReloadTimerHandle;
	FTimerHandle FireSpecialTimerHandle;

	bool bReadyToFire = true;
};
