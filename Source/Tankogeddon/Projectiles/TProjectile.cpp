
#include "TProjectile.h"

#include "Tankogeddon/Enemies/TTurret.h"
#include "Tankogeddon/Interfaces/IDamageTaker.h"
#include "Tankogeddon/Tank/TankPawn.h"


ATProjectile::ATProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	SetRootComponent(CapsuleComponent);

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMeshComponent->SetupAttachment(CapsuleComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &ATProjectile::OnBounce);
}

void ATProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	// if(auto damageTaker = Cast<IIDamageTaker>(ImpactResult.GetActor()))
	// {
	// 	FTDamageData DamageData;
	// 	DamageData.DamageValue = Damage;
	// 	DamageData.Causer = this;
	// 	DamageData.Instigator = this;
	//
	// 	// if(const auto turretTaker = Cast<ATTurret>(damageTaker))
	// 	// 	turretTaker->TakeDamage(DamageData);
	// 	// else if (const auto tankTaker = Cast<ATankPawn>(damageTaker))
	// 	// 	tankTaker->TakeDamage(DamageData);
	//
	// 	damageTaker->TakeDamage(DamageData);
	// }
	if(auto damageTaker = Cast<ATTurret>(ImpactResult.GetActor()))
	{
		FTDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.Causer = this;
		DamageData.Instigator = this;

		// if(const auto turretTaker = Cast<ATTurret>(damageTaker))
		// 	turretTaker->TakeDamage(DamageData);
		// else if (const auto tankTaker = Cast<ATankPawn>(damageTaker))
		// 	tankTaker->TakeDamage(DamageData);

		damageTaker->TakeDamage(DamageData);
	}
	if(auto damageTaker = Cast<ATankPawn>(ImpactResult.GetActor()))
	{
		FTDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.Causer = this;
		DamageData.Instigator = this;

		// if(const auto turretTaker = Cast<ATTurret>(damageTaker))
		// 	turretTaker->TakeDamage(DamageData);
		// else if (const auto tankTaker = Cast<ATankPawn>(damageTaker))
		// 	tankTaker->TakeDamage(DamageData);

		damageTaker->TakeDamage(DamageData);
	}
	Destroy();
}
