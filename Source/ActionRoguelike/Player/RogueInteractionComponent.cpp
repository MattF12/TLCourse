// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

#include "RogueGameTypes.h"
#include "Core/RogueInteractionInterface.h"
#include "Engine/OverlapResult.h"

TAutoConsoleVariable<bool> CVarInteractionDebugDrawing(TEXT("game.interaction.DebugDraw"), false,
	TEXT("Enable interaction debug rendering. (0 = off, 1 = enabled"), 
	ECVF_Cheat);

URogueInteractionComponent::URogueInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void URogueInteractionComponent::Interact()
{
	// if (IRogueInteractionInterface* InteractionInterface = Cast<IRogueInteractionInterface>(SelectedActor))
	// {
	// 	InteractionInterface->Interact();
	// }
	
	IRogueInteractionInterface::Execute_Interact(SelectedActor);
}

void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Our owner is the PlayerController.
    APlayerController* PC = CastChecked<APlayerController>(GetOwner());

    // Player's world position (used as the center of the interaction sphere).
    FVector Center = PC->GetPawn()->GetActorLocation();

    // Camera position (used for "what is the player looking at?" calculations).
    FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();

    ECollisionChannel CollisionChannel = COLLISION_INTERACTION;

    // Create a sphere that represents our interaction range.
    FCollisionShape Shape;
    Shape.SetSphere(InteractionRadius);

    // Precompute radius² so we can compare squared distances
    // without performing expensive square root operations.
    float InteractionRadiusSqrd = InteractionRadius * InteractionRadius;

    // Find all interactable actors inside the interaction sphere.
    TArray<FOverlapResult> Overlaps;
    GetWorld()->OverlapMultiByChannel(
        Overlaps,
        Center,
        FQuat::Identity,
        CollisionChannel,
        Shape);

    AActor* BestActor = nullptr;

    // Tracks the highest score found so far.
    float HighestWeight = 0.0f;

    bool bEnabledDebugDraw =
        CVarInteractionDebugDrawing.GetValueOnGameThread();

    for (FOverlapResult& Overlap : Overlaps)
    {
        // Get the center of the actor's bounding box.
        //
        // This is usually a better representation of where the object
        // visually appears than GetActorLocation(), since pivots can
        // be offset from the visible geometry.
        FVector Origin;
        FVector BoxExtends;
        Overlap.GetActor()->GetActorBounds(
            true,
            Origin,
            BoxExtends);

        // Direction from the camera to the actor.
        //
        // GetSafeNormal() converts the vector into a unit vector
        // (length = 1), which is required for meaningful dot products.
        FVector OverlapDirection =
            (Origin - CameraLocation).GetSafeNormal();

        // Squared distance from the player to the actor.
        //
        // SizeSquared() avoids a square root and is much faster than Size().
        float DistanceToSqrd =
            (Origin - Center).SizeSquared();

        // Convert distance into a score between 0 and 1.
        //
        // DistanceToSqrd:
        //      0                     -> actor is at player
        //      InteractionRadiusSqrd -> actor is at edge of range
        //
        // Divide by radius²:
        //      0 -> 1
        //
        // Then invert:
        //      1 = very close
        //      0 = very far
        float NormalizedDistanceTo =
            1.0f - (DistanceToSqrd / InteractionRadiusSqrd);

        // Compare the player's look direction against the direction
        // toward this actor.
        //
        // Dot Product results:
        //      1.0 = perfectly centered in view
        //      0.0 = 90 degrees to the side
        //     -1.0 = directly behind player
        float DotResult =
            FVector::DotProduct(
                OverlapDirection,
                PC->GetControlRotation().Vector());

        // Remap the dot product from [-1, 1] to [0, 1].
        //
        // This lets us combine it with our distance score since
        // both values now use the same range.
        //
        // Examples:
        //     -1 -> 0
        //      0 -> 0.5
        //      1 -> 1
        float NormalizedDotResult =
            DotResult * 0.5f + 0.5f;

        // Final weighted score.
        //
        // DirectionWeightScale controls how important screen-center
        // alignment is.
        //
        // DistanceToWeightScale controls how important proximity is.
        //
        // Larger score = better interaction candidate.
        float Weight =
            (NormalizedDotResult * DirectionWeightScale)
            +
            (NormalizedDistanceTo * DistanceToWeightScale);

        // Keep whichever actor currently has the highest score.
        if (Weight > HighestWeight)
        {
            BestActor = Overlap.GetActor();
            HighestWeight = Weight;
        }

        if (bEnabledDebugDraw)
        {
            DrawDebugBox(GetWorld(),Origin,FVector(50.0f), FColor::Red);

            FString DebugString =
                FString::Printf(
                    TEXT("Weight: %f, Dot: %f, Dist: %f"),
                    Weight,
                    NormalizedDotResult,
                    NormalizedDistanceTo);

            DrawDebugString(
                GetWorld(),
                Origin,
                DebugString,
                nullptr,
                FColor::White,
                0.0f,
                true);
        }
    }

    // Store the highest-scoring interaction target.
    SelectedActor = BestActor;

    if (bEnabledDebugDraw)
    {
        if (BestActor)
        {
            // Green box shows the currently selected actor.
            DrawDebugBox(
                GetWorld(),
                BestActor->GetActorLocation(),
                FVector(60.0f),
                FColor::Green);
        }

        // Visualize the interaction range.
        DrawDebugSphere(
            GetWorld(),
            Center,
            InteractionRadius,
            32,
            FColor::White);
    }
}