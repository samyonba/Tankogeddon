// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Tankogeddon/Cannon/TCannon.h"
#include "Tankogeddon/Core/TPawn.h"

#include "TankPawn.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATCannon;


UCLASS()
class TANKOGEDDON_API ATankPawn : public ATPawn
{
	GENERATED_BODY()
	
public:
	ATankPawn();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetCannon(TSubclassOf<ATCannon> CannonClass) override;
	void ChangeCannon();
	virtual void TakeDamage(const FTDamageData& DamageData) override;

public:
	
	
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSpeed = 512;
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveAcceleration = 512;

	UPROPERTY(EditAnywhere, Category="Movement")
	float RotationSpeed = 60;
	UPROPERTY(EditAnywhere, Category="Movement")
	float RotationAcceleration = 45;

	UPROPERTY(EditAnywhere, Category="Movement")
	float StrafeSpeed = 400;
	UPROPERTY(EditAnywhere, Category="Movement")
	float StrafeAcceleration = 300;

	UPROPERTY(EditAnywhere, Category="Movement")
	bool bReverseRotation = true;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;
	
protected:
	virtual USceneComponent* GetComponentToAttachCannon() const override;
	
	void OnMoveForward(float AxisValue);
	void OnMoveRight(float AxisValue);
	void OnStrafeRight(float AxisValue);

	void PerformMovement(float DeltaTime);
	void PerformTurretRotation() const;

	void PrintAmmunition() const;

protected:
	float MoveForwardInput;
	float MoveRightInput;
	float StrafeRightInput;

	float CurrentMoveSpeed = 0;
	float CurrentRotationSpeed = 0;
	float CurrentStrafeSpeed = 0;
	
};
