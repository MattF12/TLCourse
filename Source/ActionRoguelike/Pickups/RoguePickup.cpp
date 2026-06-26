#include "RoguePickup.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "RogueGameTypes.h"

ARoguePickup::ARoguePickup()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionObjectType(COLLISION_PICKUP);

	SphereComponent->SetGenerateOverlapEvents(true);
	
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

void ARoguePickup::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(
		this, &ARoguePickup::OnComponentBeginOverlap);
}

void ARoguePickup::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* Pawn = Cast<APawn>(OtherActor))
	{
		GivePickupTo(Pawn);
	}
}

void ARoguePickup::GivePickupTo(APawn* PawnToGive)
{
	// Override by derived class
}
