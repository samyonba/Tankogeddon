// Fill out your copyright notice in the Description page of Project Settings.


#include "TTripleShotCannon.h"


// Sets default values
ATDoubleShotCannon::ATDoubleShotCannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATDoubleShotCannon::Fire()
{
	if(ProjectileClass != nullptr)
	{
		

		if(!IsReadyToFire() || FireAmmunition <= 0)
		{
			return;
		}
		else
		{
			bReadyToFire = false;
			FTimerHandle SecondShotDelay;
			FTimerHandle ThirdShotDelay;
			DoSingleShot();
			GetWorld()->GetTimerManager().SetTimer(SecondShotDelay, this, &ATDoubleShotCannon::DoSingleShot, 0.2, false, 0.2);
			GetWorld()->GetTimerManager().SetTimer(ThirdShotDelay, this, &ATDoubleShotCannon::DoSingleShot, 0.2, false, 0.4);
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ATDoubleShotCannon::SetReadyToFire, ReloadTime, false);
			FireAmmunition--;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("ATDoubleShotCannon: ProjectileClass is null!"));
	}
}

void ATDoubleShotCannon::DoSingleShot()
{
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ATProjectile* Projectile = GetWorld()->SpawnActor<ATProjectile>(ProjectileClass, FirePoint->GetComponentLocation(), FirePoint->GetComponentRotation(), ActorSpawnParameters);
}



