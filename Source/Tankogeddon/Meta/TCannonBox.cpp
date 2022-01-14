
#include "TCannonBox.h"

#include "Tankogeddon/Tank/TankPawn.h"

ATCannonBox::ATCannonBox()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATCannonBox::OnBeginOverlap);
	
	BoxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BoxMeshComponent");
	BoxMeshComponent->SetupAttachment(BoxComponent);
}

void ATCannonBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(auto pawn = Cast<ATankPawn>(OtherActor))
	{
		pawn->SetCannon(CannonClass);

		// Destroy();
	}
}
