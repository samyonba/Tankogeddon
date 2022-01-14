// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "Tankogeddon/Cannon/TCannon.h"
#include "Tankogeddon/Components/THealthComponent.h"

#include "TPawn.generated.h"

class UStaticMeshComponent;
class ATCannon;


UCLASS()
class TANKOGEDDON_API ATPawn : public APawn, public IIDamageTaker
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATCannon> DefaultCannonClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY()
	ATCannon* Cannon;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* CannonPivotComponent;

	UPROPERTY()
	ATCannon* AdditionalCannon;

	UPROPERTY(EditAnywhere)
	UTHealthComponent* HealthComponent;
	
public:
	ATPawn();
	virtual void BeginPlay() override;
	
	void Fire();
	void StartFireSpecial();
	void StopFireSpecial();
	virtual void SetCannon(TSubclassOf<ATCannon> CannonClass);

	//~ Begin IDamageTaker Interface
	virtual void TakeDamage(const FTDamageData& DamageData) override;
	//~ End IIDamageTaker Interface
	
protected:
	virtual USceneComponent* GetComponentToAttachCannon() const;

	UFUNCTION()
	virtual void OnDie();
};
