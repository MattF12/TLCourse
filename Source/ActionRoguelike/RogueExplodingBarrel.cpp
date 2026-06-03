// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueExplodingBarrel.h"

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
	PrimaryActorTick.bCanEverTick = false;
	
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	BarrelMesh->SetupAttachment(RootComponent);
	
	// Physics
	BarrelMesh->SetSimulatePhysics(true);
	BarrelMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BarrelMesh->SetNotifyRigidBodyCollision(true);
	BarrelMesh->SetCollisionProfileName("PhysicsActor");
	
	// Radial force
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->ImpulseStrength = 2500.0f;
	RadialForceComponent->ForceStrength = 150000.0f;
	RadialForceComponent->Radius = 750.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bIgnoreOwningActor = true;
	
	// VFX (particle effects, Niagara systems)
	VFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFXComponent"));
	VFXComponent->SetupAttachment(RootComponent);
	FuseVFXLoop = nullptr;
	ExplosionVFX = nullptr;
	
	// Audio
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	FuseAudioLoop = nullptr;
	ExplosionAudio = nullptr;
	
	/* Initialize values */

	// Fuse
	FuseDamageThresholdToTrigger = 1.0f;	// 1.0 damage to trigger fuse
	FuseTimeToExplode = 3.0f;				// Fuse Timer Delay: 3.0 seconds
	bFuseTriggered = false;					// Initial value: false
	bExplosionTriggered = false;			// Initial value: false
}

float ARogueExplodingBarrel::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount >= FuseDamageThresholdToTrigger && bFuseTriggered == false)
	{
		OnFuseStart();
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ARogueExplodingBarrel::OnFuseStart()
{
	bFuseTriggered = true;		// Set flag: bFuseTriggered = true
	PlayFuseEffects(true);		// Play fuse VFX and Audio
	
	// Set Timer to call OnFuseEnd() when complete
	GetWorldTimerManager().SetTimer(FuseTimerHandle, this, &ARogueExplodingBarrel::OnFuseEnd, FuseTimeToExplode, false);
}

void ARogueExplodingBarrel::OnFuseEnd()
{
	PlayFuseEffects(false);									// Stop fuse VFX and Audio
	GetWorldTimerManager().ClearTimer(FuseTimerHandle);	// Clear Timer
	OnExplode();											// Call OnExplode()
}

void ARogueExplodingBarrel::OnExplode()
{
	bExplosionTriggered = true;					// Set flag: bExplosionTriggered = true
	VFXComponent->SetAsset(ExplosionVFX);	// VFX Component: Set as ExplosionVFX 
	PlayExplosionEffects();						// Play Explosion VFX and Audio
	// Apply Radial Force
	RadialForceComponent->FireImpulse();
}

void ARogueExplodingBarrel::PlayFuseEffects(const bool bPlay)
{
	if (bPlay == true)
	{
		if (FuseVFXLoop)
		{
			VFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
					FuseVFXLoop, 
					BarrelMesh, 
					NAME_None, 
					FVector::ZeroVector, 
					FRotator::ZeroRotator, 
					EAttachLocation::SnapToTarget, 
					true);
		}
		if (FuseAudioLoop)
		{
			AudioComponent = UGameplayStatics::SpawnSoundAttached(
				FuseAudioLoop, BarrelMesh);
		}
	}
	else
	{
		if (VFXComponent)
		{
			VFXComponent->Deactivate();
		}
		if (FuseAudioLoop)
		{
			AudioComponent->Deactivate();
		}
	}
}

void ARogueExplodingBarrel::PlayExplosionEffects()
{
	// Play Explosion VFX
	if (ExplosionVFX)
	{
		VFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ExplosionVFX,
			BarrelMesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			false);
	}
	
	// Play Explosion Audio
	if (ExplosionAudio)
	{
		AudioComponent = UGameplayStatics::SpawnSoundAttached(
			ExplosionAudio, BarrelMesh);
	}
}

// Called when the game starts or when spawned
void ARogueExplodingBarrel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARogueExplodingBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
