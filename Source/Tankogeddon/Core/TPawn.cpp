
#include "TPawn.h"
#include "Tankogeddon/Cannon/TCannon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"


ATPawn::ATPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComponent"));
	BodyMesh->SetupAttachment(BoxComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMeshComponent"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonPivotComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonPivotComponent"));
	CannonPivotComponent->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UTHealthComponent>("HealthComponent");
	HealthComponent->OnDieDelegate.AddUObject(this, &ATPawn::OnDie);
	HealthComponent->SetMaxHealth(60);
}

void ATPawn::BeginPlay()
{
	Super::BeginPlay();

	SetCannon(DefaultCannonClass);
}

void ATPawn::Fire()
{
	if(Cannon)
	{
		Cannon->Fire();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("No cannon"));
	}
}

void ATPawn::StartFireSpecial()
{
	if(Cannon)
	{
		Cannon->StartFireSpecial();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("No cannon"));
	}
}

void ATPawn::StopFireSpecial()
{
	if(Cannon)
	{
		Cannon->StopFireSpecial();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("No cannon"));
	}
}

void ATPawn::SetCannon(const TSubclassOf<ATCannon> CannonClass)
{
	if(CannonClass != nullptr)
	{
		if(Cannon != nullptr)
			Cannon->Destroy();

		const auto ComponentToAttachCannon = GetComponentToAttachCannon();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Cannon = GetWorld()->SpawnActor<ATCannon>(CannonClass, ComponentToAttachCannon->GetComponentLocation(),
		                                          ComponentToAttachCannon->GetComponentRotation(), SpawnParameters);
		Cannon->AttachToComponent(ComponentToAttachCannon, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void ATPawn::TakeDamage(const FTDamageData& DamageData)
{
	
}

USceneComponent* ATPawn::GetComponentToAttachCannon() const
{
	return BodyMesh;
}

void ATPawn::OnDie()
{
	Destroy();
}

