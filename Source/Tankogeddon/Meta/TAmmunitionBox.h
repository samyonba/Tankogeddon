// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TAmmunitionBox.generated.h"

class UBoxComponent;
UCLASS()
class TANKOGEDDON_API ATAmmunitionBox : public AActor
{
	GENERATED_BODY()

public:
	ATAmmunitionBox();

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoxMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	int increaseFireAmmunitionValue = 10;

	UPROPERTY(EditAnywhere)
	int increaseSpecialFireAmmunitionValue = 50;

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
