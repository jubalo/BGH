// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "SpiderBoss.h"
#include "SpiderBossAIController.h"
#include "PaperFlipbookComponent.h"
#include "Hunter.h"
#include "Engine.h"
#include "BaseCharacter.h"

ASpiderBoss::ASpiderBoss(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIControllerClass = ASpiderBossAIController::StaticClass();
	/**
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
			: WalkUpAnimationAsset(TEXT("/Game/Miscellaneous/Spiderling/FB_Spiderling_Walk_Up"))
			, WalkDownAnimationAsset(TEXT("/Game/Miscellaneous/Spiderling/FB_Spiderling_Walk_Down"))
			, WalkLeftAnimationAsset(TEXT("/Game/Miscellaneous/Spiderling/FB_Spiderling_Walk_Left"))
			, WalkRightAnimationAsset(TEXT("/Game/Miscellaneous/Spiderling/FB_Spiderling_Walk_Right"))
			, AttackAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/Attack.Attack"))
			, IdleAnimationAsset(TEXT("/Game/Miscellaneous/Spiderling/FB_Spiderling_Idle"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	RootComponent->RelativeRotation = FRotator(0.0f, 0.0f, 90.0f);

	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollision"));
	MeleeCollisionComp->SetCapsuleHalfHeight(60);
	MeleeCollisionComp->SetCapsuleRadius(35, false);
	MeleeCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeleeCollisionComp->AttachParent = GetCapsuleComponent();

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

	MeleeDamage = 24.0f;
	MeleeStrikeCooldown = 1.0f;
	*/
}


