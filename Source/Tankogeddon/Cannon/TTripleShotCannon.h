// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TCannon.h"
#include "GameFramework/Actor.h"
#include "TTripleShotCannon.generated.h"

UCLASS()
class TANKOGEDDON_API ATDoubleShotCannon : public ATCannon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATDoubleShotCannon();
	virtual void Fire() override;

protected:
	void DoSingleShot();
};
