// Fill out your copyright notice in the Description page of Project Settings.


#include "TCannon.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ATCannon::ATCannon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	TransformComponent = CreateDefaultSubobject<USceneComponent>("TransformComponent");
	SetRootComponent(TransformComponent);

	// CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>("CannonMesh");
	// CannonMesh->SetupAttachment(TransformComponent);

	FirePoint = CreateDefaultSubobject<UArrowComponent>("FirePoint");
	FirePoint->SetupAttachment(TransformComponent);
}

void ATCannon::Fire()
{
	if(ProjectileClass != nullptr)
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if(!IsReadyToFire() || FireAmmunition <= 0)
		{
			return;
		}
		else
		{
			ReloadCannon();
			ATProjectile* Projectile = GetWorld()->SpawnActor<ATProjectile>(ProjectileClass, FirePoint->GetComponentLocation(), FirePoint->GetComponentRotation(), ActorSpawnParameters);
			FireAmmunition--;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("ATCannon: ProjectileClass is null!"));
	}
}

void ATCannon::StartFireSpecial()
{
	// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("StartFireSpecial"));

	if(FireSpecialAmmunition <= 0)
		return;
	else
	{
		GetWorld()->GetTimerManager().SetTimer(FireSpecialTimerHandle, this, &ATCannon::DoFireSpecial,
		                                       FireSpecialReloadTime, true,
		                                       FireSpecialReloadTime);
	}

}

void ATCannon::StopFireSpecial()
{
	// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("StopFireSpecial"));

	GetWorld()->GetTimerManager().ClearTimer(FireSpecialTimerHandle);
}

void ATCannon::DoFireSpecial()
{
	// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("FireSpecial"));
	
	if(FireSpecialAmmunition == 0)
	{
		return;
	}
	FireSpecialAmmunition--;
	
	const FVector startTrace = FirePoint->GetComponentLocation();
	const FVector endTrace = startTrace + FirePoint->GetForwardVector() * TraceFireRange;

	const EDrawDebugTrace::Type drawDebugType = bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(this, startTrace, endTrace, TraceChannel, false, {}, drawDebugType, HitResult, true);
}

bool ATCannon::IsReadyToFire()
{
	return bReadyToFire;
}

void ATCannon::SetReadyToFire()
{
	bReadyToFire = true;
}

int ATCannon::GetFireAmmunition() const
{
	return FireAmmunition;
}

int ATCannon::GetSpecialFireAmmunition() const
{
	return FireSpecialAmmunition;
}

void ATCannon::SetNotReadyToFire()
{
	bReadyToFire = false;
}

void ATCannon::IncreaseFireAmmunition(int value)
{
	FireAmmunition += value;
}

void ATCannon::IncreaseSpecialFireAmmunition(int value)
{
	FireSpecialAmmunition += value;
}

void ATCannon::ReloadCannon()
{
	SetNotReadyToFire();
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ATCannon::SetReadyToFire, ReloadTime, false);
}

