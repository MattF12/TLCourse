// Fill out your copyright notice in the Description page of Project Settings.


#include "World/RogueExplodingBarrel.h"

#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
ARogueExplodingBarrel::ARogueExplodingBarrel()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetSimulatePhysics(true);
	// Blueprint automatically sets this collision profile when enabling simulate physics, in C++ we do this manually.
	MeshComponent->SetCollisionProfileName("PhysicsActor");
	MeshComponent->SetCanEverAffectNavigation(false);
	RootComponent = MeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComponent->SetupAttachment(MeshComponent);
	// Must be quite large, depends on the Mass of the Actors we intend to launch such as the Cubes
	RadialForceComponent->ImpulseStrength = 150000.0f;
	RadialForceComponent->Radius = 750.f;
	// We don't need to Activate this component, that is only good for applying a constant radial force and not our desired one frame Impulse
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = true;
}

float ARogueExplodingBarrel::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Allow exploding once, also skip when timer is already running
	if (bExploded || GetWorldTimerManager().TimerExists(ExplosionTimerHandle))
	{
		return ActualDamage;
	}

	// Hold onto both the Niagara and Audio Components to disable them during the Explode()
	
	ActiveBurningEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(BurningEffect, MeshComponent, NAME_None,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);

	ActiveBurningSoundComp = UGameplayStatics::SpawnSoundAttached(BurningSound, MeshComponent);
	
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ARogueExplodingBarrel::Explode, ExplosionDelay);

	return ActualDamage;
}

void ARogueExplodingBarrel::Explode()
{
	bExploded = true;

	if (ActiveBurningEffectComp)
	{
		ActiveBurningEffectComp->Deactivate();
	}
	if (ActiveBurningSoundComp)
	{
		ActiveBurningSoundComp->Stop();
	}

	RadialForceComponent->FireImpulse();

	MeshComponent->AddImpulse(FVector::UpVector * 1000, NAME_None, true);
	MeshComponent->AddAngularImpulseInDegrees(FVector::RightVector * 1000, NAME_None, true);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation(), GetActorRotation());

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
}