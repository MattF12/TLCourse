#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoguePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USoundBase;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARoguePickup : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> OverlapComponent;
	
	UFUNCTION()
	virtual void OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	virtual void PostInitializeComponents() override;
	
	ARoguePickup();
};
