// Fill out your copyright notice in the Description page of Project Settings.


#include "TAmmunitionBox.h"

#include "Components/BoxComponent.h"
#include "Tankogeddon/Tank/TankPawn.h"


// Sets default values
ATAmmunitionBox::ATAmmunitionBox()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);

	BoxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BoxMeshComponent");
	BoxMeshComponent->SetupAttachment(BoxComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATAmmunitionBox::OnBeginOverlap);
}

void ATAmmunitionBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(const auto pawn = Cast<ATankPawn>(OtherActor))
	{
		pawn->Cannon->IncreaseFireAmmunition(increaseFireAmmunitionValue);
		pawn->Cannon->IncreaseSpecialFireAmmunition(increaseSpecialFireAmmunitionValue);

		Destroy();
	}
}
