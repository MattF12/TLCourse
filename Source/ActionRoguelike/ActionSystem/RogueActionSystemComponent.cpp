// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}

bool URogueActionSystemComponent::ApplyHeathChange(float InValueChange)
{
	bool bHealthChanged = false;
	const float OldHealth = Attributes.Health;
	
	Attributes.Health = FMath::Clamp(Attributes.Health + InValueChange, 0.0f,  Attributes.MaxHealth);
	
	if (IsFullHealth() == false)
	{
		OnHealthChanged.Broadcast(Attributes.Health, OldHealth);
		
		bHealthChanged = true;
	}

	UE_LOG(LogTemp, Log, TEXT("New Health: %f, Max Health: %f"), Attributes.Health, Attributes.MaxHealth);
	return bHealthChanged;
}

bool URogueActionSystemComponent::IsFullHealth() const
{
	return FMath::IsNearlyEqual(Attributes.Health, Attributes.MaxHealth);
}



