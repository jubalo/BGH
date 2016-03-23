// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Hunter.h"


// Sets default values
AHunter::AHunter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> LeftSideRunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> RightSideRunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> UpRunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> DownRunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleDownAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleUpAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleLeftAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleRightAnimationAsset;
		FConstructorStatics():
			IdleDownAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Idle_Down")),
			IdleUpAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Idle_Up")),
			IdleLeftAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Idle_Left")),
			IdleRightAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Idle_Right")),
			LeftSideRunningAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Walk_Side_Left")),
			RightSideRunningAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Walk_Side_Right")),
			UpRunningAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Walk_Up")),
			DownRunningAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Walk_Down"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	LeftSideRunningAnimation = ConstructorStatics.LeftSideRunningAnimationAsset.Get();
	RightSideRunningAnimation = ConstructorStatics.RightSideRunningAnimationAsset.Get();
	UpRunningAnimation = ConstructorStatics.UpRunningAnimationAsset.Get();
	DownRunningAnimation = ConstructorStatics.DownRunningAnimationAsset.Get();
	IdleDownAnimation = ConstructorStatics.IdleDownAnimationAsset.Get();
	IdleUpAnimation = ConstructorStatics.IdleUpAnimationAsset.Get();
	IdleLeftAnimation = ConstructorStatics.IdleLeftAnimationAsset.Get();
	IdleRightAnimation = ConstructorStatics.IdleRightAnimationAsset.Get();
	GetSprite()->SetFlipbook(IdleDownAnimation);

	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HunterBase"));
	}


	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(29.0f);
	GetCapsuleComponent()->SetCapsuleRadius(16.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	CameraBoom->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	// Characteristics
	GetCharacterMovement()->GroundFriction = 20.0f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 4000.0f;

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 800.0f;
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);

	Orientation = 3;
}

// Called when the game starts or when spawned
void AHunter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCharacter();
}

void AHunter::HorizontalMove(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AHunter::VerticalMove(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
}

// Called to bind functionality to input
void AHunter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	InputComponent->BindAxis("Side_Running", this, &AHunter::HorizontalMove);
	InputComponent->BindAxis("Vertical_Running", this, &AHunter::VerticalMove);
}

void AHunter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirectionX = PlayerVelocity.X;
	float TravelDirectionY = PlayerVelocity.Y;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirectionX < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 0.0f, 0.0f));
		}
		else if (TravelDirectionX > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 180.0f));
		}
	}
}

void AHunter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	bool PlayerOrientation = true;
	UPaperFlipbook* DesiredAnimation = IdleDownAnimation;

	// Are we moving horizontally?
	if (PlayerVelocity.X != 0.0f) {
		PlayerOrientation = true;
		
		//Are we moving left?
		if (PlayerVelocity.X < 0.0f) {
			Orientation = 0;
			DesiredAnimation = LeftSideRunningAnimation;
		}
		else {
			Orientation = 2;
			DesiredAnimation = RightSideRunningAnimation;
		}
	}

	// Are we moving Vertically?
	else if (PlayerVelocity.Y != 0.0f) {
		PlayerOrientation = false;
		
		// Are we moving Down?
		if (PlayerVelocity.Y < 0.0f) {
			Orientation = 1;
			DesiredAnimation = UpRunningAnimation;
		}
		else {
			Orientation = 3;
			DesiredAnimation = DownRunningAnimation;
		}
	}

	// We are not moving
	else {
		if (Orientation == 0) {
			DesiredAnimation = IdleLeftAnimation;
		}
		else if (Orientation == 2) {
			DesiredAnimation = IdleRightAnimation;
		}
		else if (Orientation == 1) {
			DesiredAnimation = IdleUpAnimation;
		}
		else {
			DesiredAnimation = IdleDownAnimation;
		}
	}

	//Update sprite
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}

	
	/** /
	const float PlayerSpeed = PlayerVelocity.Size();
	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeed > 0.0f) ? SideRunningAnimation : IdleDownAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
	/**/
}


