// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Tankogeddon/Cannon/TCannon.h"
#include "DrawDebugHelpers.h"
#include "Tankogeddon/Player/TankPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"


// Sets default values
ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;



	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	SetCannon(DefaultCannonClass);
}

USceneComponent* ATankPawn::GetComponentToAttachCannon() const
{
	return CannonPivotComponent;
}

void ATankPawn::OnMoveForward(const float AxisValue)
{
	MoveForwardInput = AxisValue;
}

void ATankPawn::OnMoveRight(const float AxisValue)
{
	MoveRightInput = AxisValue;
}

void ATankPawn::OnStrafeRight(const float AxisValue)
{
	StrafeRightInput = AxisValue;
}

void ATankPawn::PerformMovement(const float DeltaTime)
{
	float k = 1.f; // используется для нормирования максимальной скорости танка при одновременном движении и стрейфе
	if (FMath::Abs(MoveForwardInput) == 1 && FMath::Abs(StrafeRightInput) == 1)
		k /= FMath::Sqrt(2);
	
	CurrentMoveSpeed = FMath::FInterpConstantTo(CurrentMoveSpeed, MoveSpeed * MoveForwardInput * k, DeltaTime, MoveAcceleration);
	const FVector MoveDelta = CurrentMoveSpeed * GetActorForwardVector() * DeltaTime;
	SetActorLocation(GetActorLocation() + MoveDelta);

	CurrentStrafeSpeed = FMath::FInterpConstantTo(CurrentStrafeSpeed, StrafeSpeed * StrafeRightInput * k, DeltaTime, StrafeAcceleration);
	const FVector StrafeDelta = CurrentStrafeSpeed * GetActorRightVector() * DeltaTime;
	SetActorLocation(GetActorLocation() + StrafeDelta);
	
	float ReverseRotation = (MoveForwardInput == -1 && bReverseRotation) ? -1 : 1; // реверсивный поворот при движении назад
	CurrentRotationSpeed = FMath::FInterpConstantTo(CurrentRotationSpeed, RotationSpeed * FMath::Abs(MoveRightInput), DeltaTime, RotationAcceleration);
	const FRotator RotationDelta = FRotator(0, CurrentRotationSpeed * DeltaTime * FMath::Sign(MoveRightInput) * ReverseRotation, 0);
	SetActorRotation(GetActorRotation() + RotationDelta);
}

void ATankPawn::PerformTurretRotation() const
{
	if(const auto controller = Cast<APlayerController>(GetController()))
	{
		float MouseX, MouseY;
		controller->GetMousePosition(MouseX, MouseY);

		FVector MouseLocation, MouseDirection;
		controller->DeprojectScreenPositionToWorld(MouseX, MouseY, MouseLocation, MouseDirection);

		//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Orange, FString::Printf(TEXT("Mouse X: %.2f, Mouse Y: %.2f"), MouseX, MouseY));
		//DrawDebugSphere(GetWorld(), MouseLocation, 15, 32, FColor::Green);

		const FVector ActorLocation = GetActorLocation();
		const float k = (ActorLocation.Z - MouseLocation.Z)/MouseDirection.Z;
		const FVector CorrectMouseLocation = MouseLocation + (MouseDirection * k);
		FVector CorrectMouseDirection = (CorrectMouseLocation - ActorLocation);
		CorrectMouseDirection.Normalize();
		// DrawDebugLine(GetWorld(), ActorLocation, CorrectMouseLocation, FColor::Green, false, 0, 0, 5);

		const FRotator NewTurretRotation = UKismetMathLibrary::FindLookAtRotation(ActorLocation, CorrectMouseLocation);
		// NewTurretRotation.Roll = TurretMesh->GetComponentRotation().Roll;
		// NewTurretRotation.Pitch = TurretMesh->GetComponentRotation().Pitch;
		TurretMesh->SetWorldRotation(NewTurretRotation);
	}
}

void ATankPawn::PrintAmmunition() const
{
	if(Cannon != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green,
										 FString::Printf(
											 TEXT("Fire Ammunition: %.2i, Fire Special Ammunition: %.3i"), Cannon->GetFireAmmunition(),
											 Cannon->GetSpecialFireAmmunition()));
	}
}

void ATankPawn::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformMovement(DeltaTime);
	PerformTurretRotation();

	PrintAmmunition();
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATankPawn::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATankPawn::OnMoveRight);
	PlayerInputComponent->BindAxis("StrafeRight", this, &ATankPawn::OnStrafeRight);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATankPawn::Fire);
	PlayerInputComponent->BindAction("FireSpecial", EInputEvent::IE_Pressed, this, &ATankPawn::StartFireSpecial);
	PlayerInputComponent->BindAction("FireSpecial", EInputEvent::IE_Released, this, &ATankPawn::StopFireSpecial);

	PlayerInputComponent->BindAction("ChangeCannon", EInputEvent::IE_Pressed, this, &ATankPawn::ChangeCannon);
}

void ATankPawn::SetCannon(const TSubclassOf<ATCannon> CannonClass)
{
	if(CannonClass != nullptr)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (Cannon == nullptr)
		{
			Cannon = GetWorld()->SpawnActor<ATCannon>(CannonClass, CannonPivotComponent->GetComponentLocation(),
											 CannonPivotComponent->GetComponentRotation(), SpawnParameters);
			Cannon->AttachToComponent(CannonPivotComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
		else
		{
			if(CannonClass == Cannon->GetClass())
				return;
			if(AdditionalCannon != nullptr)
			{
				if(CannonClass == AdditionalCannon->GetClass())
				{
					ChangeCannon();
					return;
				}
			}
			AdditionalCannon = Cannon;
			Cannon = GetWorld()->SpawnActor<ATCannon>(CannonClass, CannonPivotComponent->GetComponentLocation(),
											 CannonPivotComponent->GetComponentRotation(), SpawnParameters);
			Cannon->AttachToComponent(CannonPivotComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
	}
}

void ATankPawn::ChangeCannon()
{
	if(Cannon != nullptr && AdditionalCannon != nullptr)
	{
		Cannon->StopFireSpecial();
		
		ATCannon* tmpCannon = Cannon;
		Cannon = AdditionalCannon;
		AdditionalCannon = tmpCannon;

		Cannon->ReloadCannon();
	}
}

void ATankPawn::TakeDamage(const FTDamageData& DamageData)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("Pawn health: %.2f"), HealthComponent->GetHealth()));
	HealthComponent->TakeDamage(DamageData);
}

