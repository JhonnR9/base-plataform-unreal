// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPaperCharacter.h"
#include "Components/InputComponent.h"
#include "Engine/EngineTypes.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/ArrowComponent.h"

APlayerPaperCharacter::APlayerPaperCharacter() {
	//initialize Camera and SpringArm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 100.f;


	//initialize Camera Component and setup
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComponent->SetupAttachment(SpringArmComponent);

	CurrentAnimation = EAnimationType::IDLE;

	if(auto ArrowCompon = GetArrowComponent())
	{
		ArrowCompon->DestroyComponent();
	}
}

void APlayerPaperCharacter::BeginPlay() {
	Super::BeginPlay();


	//Add Input Mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			constexpr int Priority = 1;
			Subsystem->AddMappingContext(DefaultMappingContext, Priority);
	
		}
	}

}

void APlayerPaperCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	UInputAction** JumpAction = InputsMap.Find(EInputType::JUMP_ACTION);
	UInputAction** MoveAction = InputsMap.Find(EInputType::MOVE_ACTION);
	UInputAction** AttackAction = InputsMap.Find(EInputType::ATTACK_ACTION);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		//Jumping
		if (!JumpAction) {
			UE_LOG(LogTemp, Error, TEXT("The input JumpAction not found"));
			return; 
		}
		EnhancedInputComponent->BindAction(*JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(*JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		if (!MoveAction) {
			UE_LOG(LogTemp, Error, TEXT("The input MoveAction not found"));
			return;
		}
		EnhancedInputComponent->BindAction(*MoveAction, ETriggerEvent::Triggered, this, &APlayerPaperCharacter::Move);

		//Attack
		if (!AttackAction) { 
			UE_LOG(LogTemp, Error, TEXT("The input AttackAction not found"));
			return;
		}
		EnhancedInputComponent->BindAction(*AttackAction, ETriggerEvent::Triggered, this,
		                                   &APlayerPaperCharacter::Attack);
	}
}

void APlayerPaperCharacter::Move(const FInputActionValue& value) {

	const FVector2D Direction = value.Get<FVector2D>();

	AddMovementInput(FVector(Direction.X, 0, 0));
}

void APlayerPaperCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	
	UpdateAnimations();


}

void APlayerPaperCharacter::UpdateAnimations()
{
	//Get defualt CharacterMovemen of APaperCharacter class
	if (const UCharacterMovementComponent* MovementComponent = GetCharacterMovement()) {
		//Get defualt PaperFlipbookComponent  of APaperCharacter class
		if (UPaperFlipbookComponent* FlipbookComponent = GetSprite())
		{
			// stores the last recorded speed and the maximum speed to be considered as stopping
			const float LastVelocityX = MovementComponent->GetLastUpdateVelocity().X;
			float MinMoveVelocity = 0.08f;
			// Convert the speed to a positive vector and compare it to the minimum speed to know if you are standing still or walking
			if (FMath::Abs(LastVelocityX) > MinMoveVelocity) {
				CurrentAnimation = EAnimationType::WALK;
			}
			else {
				CurrentAnimation = EAnimationType::IDLE;
			}

			// Check if there is a Flipbook associated with the current animation in the animations map
			if (UPaperFlipbook* UpdatedFlipbook = *AnimationsMap.Find(CurrentAnimation)) {
				// Compare the current Flipbook of the component with the updated Flipbook
				if (FlipbookComponent->GetFlipbook() != UpdatedFlipbook) {
					// If the Flipbooks are different, set the updated Flipbook in the component
					FlipbookComponent->SetFlipbook(UpdatedFlipbook);
				}
			}

			// Perform the "flip" action on the Flipbook component
			Flip(FlipbookComponent);

		}

	}
}

void APlayerPaperCharacter::Flip(UPaperFlipbookComponent* FlipbookComponent)
{
	// //Get defualt CharacterMovemen of APaperCharacter class
	if (const UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		// stores the last recorded speed
		FVector LastUpdateVelocity = MovementComponent->GetLastUpdateVelocity();

		// If the character is not moving horizontally, return without flipping
		if (LastUpdateVelocity.X == 0)
		{
			return;
		}

		// Determine if the character should be flipped based on the direction of movement
		bool bIsFlip = LastUpdateVelocity.X > 0;

		// Set the new rotation for the Flipbook component based on the flip condition
		FRotator NewRotation = bIsFlip ? FRotator(0.f, 0.f, 0.f) : FRotator(0.f, 180.f, 0.f);

		// Apply the new rotation to the Flipbook component
		FlipbookComponent->SetRelativeRotation(NewRotation);
	}
}


void APlayerPaperCharacter::Attack(const FInputActionValue& Value) {
	//not implemented yet

}



