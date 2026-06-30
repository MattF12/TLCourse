#pragma once

#include "CoreMinimal.h"
#include "RoguePickup.h"
#include "RogueHealthPickup.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARogueHealthPickup : public ARoguePickup
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category= "Components")
	TObjectPtr<UStaticMeshComponent> PickupMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	TObjectPtr<USoundBase> PickupSound;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float HealingAmount = 50.0f;

	virtual void OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	
	ARogueHealthPickup();
};
