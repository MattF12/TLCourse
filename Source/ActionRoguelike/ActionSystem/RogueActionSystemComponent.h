// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"


USTRUCT(BlueprintType)
struct FRogueAttributeSet
{
	GENERATED_BODY()
	
	FRogueAttributeSet()
	:	MaxHealth(100.0f), Health(100.0f) {}
	
	UPROPERTY(BlueprintReadOnly)
	float MaxHealth;
	
	UPROPERTY(BlueprintReadOnly)
	float Health;
};

// "DYNAMIC": Allows delegate to be exposed to Blueprints
// "MULTICAST": Allows for multiple listeners
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, OldHealth);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FRogueAttributeSet Attributes;
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	bool ApplyHeathChange(float InValueChange);

	UFUNCTION()
	bool IsFullHealth() const;
	
	URogueActionSystemComponent();
};
