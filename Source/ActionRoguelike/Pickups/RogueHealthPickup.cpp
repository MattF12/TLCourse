#include "RogueHealthPickup.h"

#include "ActionSystem/RogueActionSystemComponent.h"
#include "Player/RoguePlayerCharacter.h"

#include "Kismet/GameplayStatics.h"

ARogueHealthPickup::ARogueHealthPickup()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARogueHealthPickup::GivePickupTo(APawn* PawnToGive)
{
	// Empty method
	//Super::GivePickupTo(PawnToGive);
	
	if (ARoguePlayerCharacter* Character = Cast<ARoguePlayerCharacter>(PawnToGive))
	{
		if (URogueActionSystemComponent* ActionComp = Character->GetActionSystemComponent())
		{
			if (ActionComp->ApplyHeathChange(HealthToAdd))
			{
				// Play pickup sound
				if (PickupSound)
				{
					UGameplayStatics::PlaySound2D(this, PickupSound);
				}
				
				UE_LOG(LogTemp, Log, TEXT("Health pickup acquired!"));

				Destroy();
			}
		}
	}
}