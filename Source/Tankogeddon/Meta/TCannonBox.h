
#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Tankogeddon/Cannon/TCannon.h"
#include "TCannonBox.generated.h"

UCLASS()
class TANKOGEDDON_API ATCannonBox : public AActor
{
	GENERATED_BODY()

public:
	ATCannonBox();

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoxMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATCannon> CannonClass;

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
