#include "RoguePickup.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "RogueGameTypes.h"

ARoguePickup::ARoguePickup()
{
	OverlapComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComp"));
	// Always start with decent defaults, let Blueprint decide the final tweaked values
	OverlapComponent->SetSphereRadius(128.0f);
	RootComponent = OverlapComponent;
}

void ARoguePickup::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnActorOverlapped);
}

void ARoguePickup::OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// All behavior is defined by the derived class.
}