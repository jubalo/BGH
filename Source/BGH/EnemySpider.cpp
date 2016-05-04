// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "EnemySpider.h"
#include "SpiderAIController.h"
#include "PaperFlipbookComponent.h"
#include "Hunter.h"
#include "Engine.h"

/* AI Include */
#include "Perception/PawnSensingComponent.h"

AEnemySpider::AEnemySpider()
{
	AIControllerClass = ASpiderAIController::StaticClass();

	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> WalkUpAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> WalkDownAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> WalkLeftAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> WalkRightAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> AttackAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
		FConstructorStatics()
			: WalkUpAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/WalkUp.WalkUp"))
			, WalkDownAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/WalkDown.WalkDown"))
			, WalkLeftAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/WalkLeft.WalkLeft"))
			, WalkRightAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/WalkRight.WalkRight"))
			, AttackAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/Attack.Attack"))
			, IdleAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/idle.idle"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	RootComponent->RelativeRotation = FRotator(0.0f, 0.0f, 90.0f);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;

	WalkUpAnimation = ConstructorStatics.WalkUpAnimationAsset.Get();
	WalkDownAnimation = ConstructorStatics.WalkDownAnimationAsset.Get();
	WalkLeftAnimation = ConstructorStatics.WalkLeftAnimationAsset.Get();
	WalkRightAnimation = ConstructorStatics.WalkRightAnimationAsset.Get();
	AttackAnimation = ConstructorStatics.AttackAnimationAsset.Get();
	IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
	GetSprite()->SetFlipbook(IdleAnimation);

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(45.0f);
	GetCapsuleComponent()->SetCapsuleRadius(45.0f);

	// Set the size of our agent capsule.
	GetMovementComponent()->NavAgentProps.AgentHeight = 90.0f;
	GetMovementComponent()->NavAgentProps.AgentRadius = 45.0f;

	SenseTimeOut = 2.5f;
}

void AEnemySpider::BeginPlay()
{
	Super::BeginPlay();

	/* This is the earliest moment we can bind our delegates to the component */
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemySpider::OnSeePlayer);
	}
}

void AEnemySpider::OnSeePlayer(APawn* Pawn)
{
	ASpiderAIController* AIController = Cast<ASpiderAIController>(GetController());
	AHunter* SensedPawn = Cast<AHunter>(Pawn);
	if (AIController)
	{
		AIController->SetTargetToFollow(SensedPawn);
	}
}

void AEnemySpider::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();
	long TravelDirectionX = PlayerVelocity.X;
	long TravelDirectionY = PlayerVelocity.Y;
	int DesiredSpriteRotation = 0;

	if (PlayerSpeed == 0.0f)
	{
		GetSprite()->SetFlipbook(IdleAnimation);
		return;
	}

	if (bIsAttacking)
	{
		GetSprite()->SetFlipbook(AttackAnimation);
		return;
	}

	if (abs(TravelDirectionY) > abs(TravelDirectionX))
	{
		DesiredSpriteRotation = 1;
	}

	switch (DesiredSpriteRotation)
	{
		case 0:
			TravelDirectionX > 0.0f ? GetSprite()->SetFlipbook(WalkRightAnimation) : GetSprite()->SetFlipbook(WalkLeftAnimation);
			break;

		case 1:
			TravelDirectionY > 0.0f ? GetSprite()->SetFlipbook(WalkUpAnimation) : GetSprite()->SetFlipbook(WalkDownAnimation);
			break;

		default:
			break;
	}
}

void AEnemySpider::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCharacter();	
}

void AEnemySpider::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();
}
