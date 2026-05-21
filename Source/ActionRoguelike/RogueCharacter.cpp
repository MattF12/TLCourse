// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"

// Sets default values
ARogueCharacter::ARogueCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	//// Dont' rotate character with the camera
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationRoll = false;

	// Allow spring arm to rotate with controller
	SpringArmComponent->bUsePawnControlRotation = true;

	//// Camera should NOT rotate relative to arm
	//CameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ARogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARogueCharacter::Move);
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARogueCharacter::Look);
}

void ARogueCharacter::Move(const FInputActionValue& InValue)
{
	const FVector2D InputValue = InValue.Get<FVector2D>();

	const FVector MoveDirection = FVector(InputValue.X, InputValue.Y, 0.0f);

	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;

	// Forward/Back
	AddMovementInput(ControlRot.Vector(), InputValue.X);

	// Sideways
	FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection, InputValue.Y);
}

void ARogueCharacter::Look(const FInputActionInstance& InValue)
{
	FVector2D InputValue = InValue.GetValue().Get<FVector2D>();

	// Mouse X -> Yaw
	AddControllerYawInput(InputValue.X);

	// Mouse Y ->Pitch
	AddControllerPitchInput(InputValue.Y);
}