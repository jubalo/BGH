// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "EnemySpider.h"
#include "SpiderAIController.h"
#include "PaperFlipbookComponent.h"
#include "Hunter.h"
#include "Engine.h"
#include "BaseCharacter.h"

/* AI Include */
#include "Perception/PawnSensingComponent.h"

AEnemySpider::AEnemySpider(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->HearingThreshold = 100;
	PawnSensingComp->LOSHearingThreshold = 300;

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
	SenseTimeOut = 2.0f;
}

void AEnemySpider::BeginPlay()
{
	Super::BeginPlay();

	/* This is the earliest moment we can bind our delegates to the component */
	if (PawnSensingComp)
	{
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemySpider::OnHearNoise);
	}

	if (MeleeCollisionComp)
	{
		MeleeCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpider::OnMeleeCompBeginOverlap);
	}

	HomeLocation = GetActorLocation();
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
		TravelDirectionX < 0.0f ? GetSprite()->SetFlipbook(WalkRightAnimation) : GetSprite()->SetFlipbook(WalkLeftAnimation);
		break;

	case 1:
		TravelDirectionY < 0.0f ? GetSprite()->SetFlipbook(WalkUpAnimation) : GetSprite()->SetFlipbook(WalkDownAnimation);
		break;

	default:
		break;
	}
}

void AEnemySpider::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCharacter();

	/* Check if the last time we sensed a player is beyond the time out value to prevent bot from endlessly following a player. */
	if (bSensedTarget && (GetWorld()->TimeSeconds - LastHeardTime) > SenseTimeOut)
	{
		ASpiderAIController* AIController = Cast<ASpiderAIController>(GetController());
		if (AIController)
		{
			bSensedTarget = false;
			/* Reset */
			AIController->SetTargetToFollow(nullptr);
			AIController->SetNextWaypoint(HomeLocation);
		}
	}
}

void AEnemySpider::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();
}

void AEnemySpider::OnHearNoise(APawn* Pawn, const FVector& Location, float Volume)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SOM"));

	/* Keep track of the time the player was last sensed in order to clear the target */
	LastHeardTime = GetWorld()->GetTimeSeconds();
	bSensedTarget = true;

	ASpiderAIController* AIController = Cast<ASpiderAIController>(GetController());
	if (AIController)
	{
		AIController->SetTargetToFollow(Pawn);
	}
}

void AEnemySpider::PerformMeleeStrike(AActor* HitActor)
{
	if (LastMeleeAttackTime > GetWorld()->GetTimeSeconds() - MeleeStrikeCooldown)
	{
		/* Set timer to start attacking as soon as the cooldown elapses. */
		if (!TimerHandle_MeleeAttack.IsValid())
		{
			// TODO: Set Timer
		}

		/* Attacked before cooldown expired */
		return;
	}

	if (HitActor && HitActor != this && IsAlive())
	{
		AHunter* OtherPawn = Cast<AHunter>(HitActor);
		if (OtherPawn)
		{
			/* Set to prevent a zombie to attack multiple times in a very short time */
			LastMeleeAttackTime = GetWorld()->GetTimeSeconds();

			FPointDamageEvent DmgEvent;
			DmgEvent.Damage = MeleeDamage;

			HitActor->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);
		}
	}
}

void AEnemySpider::OnMeleeCompBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	/* Stop any running attack timers */
	TimerHandle_MeleeAttack.Invalidate();

	PerformMeleeStrike(OtherActor);

	/* Set re-trigger timer to re-check overlapping pawns at melee attack rate interval */
	GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &AEnemySpider::OnRetriggerMeleeStrike, MeleeStrikeCooldown, true);
}

void AEnemySpider::OnRetriggerMeleeStrike()
{
	/* Apply damage to a single random pawn in range. */
	TArray<AActor*> Overlaps;
	MeleeCollisionComp->GetOverlappingActors(Overlaps, ABaseCharacter::StaticClass());
	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		ABaseCharacter* OverlappingPawn = Cast<ABaseCharacter>(Overlaps[i]);
		if (OverlappingPawn)
		{
			PerformMeleeStrike(OverlappingPawn);
			//break; /* Uncomment to only attack one pawn maximum */
		}
	}

	/* No pawns in range, cancel the retrigger timer */
	if (Overlaps.Num() == 0)
	{
		TimerHandle_MeleeAttack.Invalidate();
	}
}