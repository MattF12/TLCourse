// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueItemChest.h"


ARogueItemChest::ARogueItemChest()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	StaticMeshComponent_Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	StaticMeshComponent_Base->SetCollisionProfileName("Interaction");
	RootComponent = StaticMeshComponent_Base;
	
	StaticMeshComponent_Lid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMeshComp"));
	StaticMeshComponent_Lid->SetCollisionProfileName("NoCollision");
	StaticMeshComponent_Lid->SetupAttachment(StaticMeshComponent_Base);
}

void ARogueItemChest::Interact_Implementation()
{
	// Play animation
	SetActorTickEnabled(true);
}

void ARogueItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentAnimationPitch = FMath::FInterpConstantTo(CurrentAnimationPitch, AnimationTargetPitch, DeltaTime, AnimationSpeed);
	StaticMeshComponent_Lid->SetRelativeRotation(FRotator(CurrentAnimationPitch, 0.0f, 0.0f));
	
	if (FMath::IsNearlyEqual(CurrentAnimationPitch, AnimationTargetPitch))
	{
		
		// Animation Completed
		SetActorTickEnabled(false);
		
		ChestAnimationCompleted();
	}
}

