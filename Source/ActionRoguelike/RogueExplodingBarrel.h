// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueExplodingBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class ACTIONROGUELIKE_API ARogueExplodingBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARogueExplodingBarrel();

protected:
	// Static Mesh Component (Root)
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> BarrelMesh;
	
	// Radial Force Component
	UPROPERTY(EditDefaultsOnly, Category = "Explosive Force")
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
	// Niagara Component
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UNiagaraComponent> VFXComponent;
	
	// Niagara System - Burning Fuse (loop)
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> FuseVFXLoop;
	
	// Niagara System - Explosion
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> ExplosionVFX;
	
	// Audio Component
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<UAudioComponent> AudioComponent;
	
	// Audio - Burning Fuse (loop)
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> FuseAudioLoop;
	
	// Audio - Explosion
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> ExplosionAudio;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fuse")
	float FuseDamageThresholdToTrigger;	// Default: 1.0 damage
	
	UPROPERTY(EditDefaultsOnly, Category = "Fuse")
	float FuseTimeToExplode;			// Default: 3.0 seconds
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
						class AController* EventInstigator, AActor* DamageCauser) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	bool bFuseTriggered;		// Initial value: false
	bool bExplosionTriggered;	// Initial value: false
	
	FTimerHandle FuseTimerHandle;
	
	void OnFuseStart();
	void OnFuseEnd();
	
	void OnExplode();
	
	// Helper methods
	
	void PlayFuseEffects(const bool bPlay = true);
	void PlayExplosionEffects();
};
