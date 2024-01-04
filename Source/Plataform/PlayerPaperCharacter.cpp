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
		if (AttackAction) { 
			UE_LOG(LogTemp, Error, TEXT("The input AttACKAction not found"));
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
	if (const UCharacterMovementComponent* MovementComponent = GetCharacterMovement()) {

		if (UPaperFlipbookComponent* FlipbookComponent = GetSprite())
		{
			const float LastVelocityX = MovementComponent->GetLastUpdateVelocity().X;
			float MinMoveVelocity = 0.08f;

			if (FMath::Abs(LastVelocityX) > MinMoveVelocity) {
				CurrentAnimation = EAnimationType::WALK;
			}
			else {
				CurrentAnimation = EAnimationType::IDLE;
			}

			if (UPaperFlipbook* UpdatedFlipbook = *AnimationsMap.Find((CurrentAnimation))) {
				if (FlipbookComponent->GetFlipbook() != UpdatedFlipbook) {
					FlipbookComponent->SetFlipbook(UpdatedFlipbook);
				}
			}

			Flip(FlipbookComponent);

		}

	}
}

void APlayerPaperCharacter::Flip(UPaperFlipbookComponent* FlipbookComponent)
{
	if (const UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		FVector LastUpdateVelocity = MovementComponent->GetLastUpdateVelocity();
		if (LastUpdateVelocity.X == 0) return;

		bool bIsFlip = LastUpdateVelocity.X > 0;
		FRotator NewRotation = bIsFlip ? FRotator(0.f, 0.f, 0.f) : FRotator(0.f, 180.f, 0.f);

		FlipbookComponent->SetRelativeRotation(NewRotation);
	
	}
	
}

void APlayerPaperCharacter::Attack(const FInputActionValue& Value) {
	//not implemented yet

}



