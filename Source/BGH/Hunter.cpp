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
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> SideRunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleDownAnimationAsset;
		FConstructorStatics():
			IdleDownAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Idle_Down")),
			SideRunningAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Walk_Side"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	SideRunningAnimation = ConstructorStatics.SideRunningAnimationAsset.Get();
	IdleDownAnimation = ConstructorStatics.IdleDownAnimationAsset.Get();
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
	CameraBoom->TargetArmLength = 100.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	CameraBoom->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 530.0f;
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
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

}

void AHunter::SideMove(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

// Called to bind functionality to input
void AHunter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	InputComponent->BindAxis("Side_Running", this, &AHunter::SideMove);

}

