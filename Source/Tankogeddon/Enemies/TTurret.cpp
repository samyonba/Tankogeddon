
#include "TTurret.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ATTurret::ATTurret()
{
	PrimaryActorTick.bCanEverTick = false;
	HealthComponent->SetMaxHealth(30);
}

void ATTurret::TakeDamage(const FTDamageData& DamageData)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Turret health: %.2f"), HealthComponent->GetHealth()));
	HealthComponent->TakeDamage(DamageData);
}

AActor* ATTurret::GetTarget() const
{
	return UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ATTurret::BeginPlay()
{
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	Cannon = GetWorld()->SpawnActor<ATCannon>(CannonClass, SpawnParameters);
	Cannon->AttachToComponent(TurretMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Cannon->IncreaseFireAmmunition(100);
	
	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATTurret::Targeting, TargetingRate, true,
	                                       TargetingRate);
}

// void ATTurret::Destroyed()
// {
// 	Super::Destroyed();
//
// 	if(Cannon)
// 		Cannon->Destroy();
// }

void ATTurret::Targeting()
{
	if(IsTargetInRange())
	{
		RotateToTarget();
	}

	if(CanFire() && Cannon && Cannon->IsReadyToFire())
	{
		Fire();
	}
}

void ATTurret::RotateToTarget()
{
	FRotator targetRotation =
		UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator currentRotation = TurretMesh->GetComponentRotation();

	targetRotation.Pitch = currentRotation.Pitch;
	targetRotation.Roll = currentRotation.Roll;

	TurretMesh->SetWorldRotation(FMath::Lerp(currentRotation, targetRotation, TargetingSpeed));
}

bool ATTurret::IsTargetInRange() const
{
	if(!PlayerPawn)
		return false;
	return (FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange);
}

bool ATTurret::CanFire() const
{
	FVector targetingDirection = CannonPivotComponent->GetForwardVector();
	FVector toTargetDirection = PlayerPawn->GetActorLocation() - GetActorLocation();
	toTargetDirection.Normalize();
	const float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDirection, toTargetDirection)));
	return aimAngle <= FireAccuracy;
}

void ATTurret::OnDie()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Turret OnDie")));
	this->Destroy();
}
