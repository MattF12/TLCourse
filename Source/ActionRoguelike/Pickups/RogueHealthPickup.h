#pragma once

#include "CoreMinimal.h"
#include "RoguePickup.h"
#include "RogueHealthPickup.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARogueHealthPickup : public ARoguePickup
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Value")
	float HealthToAdd = 30.0f;

	UFUNCTION()
	virtual void GivePickupTo(APawn* PawnToGive) override;
	
public:
	
	ARogueHealthPickup();
};
