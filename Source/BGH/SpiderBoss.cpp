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
			: WalkUpAnimationAsset(TEXT("/Game/Boss/Sprites/WalkUp"))
			, WalkDownAnimationAsset(TEXT("/Game/Boss/Sprites/WalkDown"))
			, WalkLeftAnimationAsset(TEXT("/Game/Boss/Sprites/WalkLeft"))
			, WalkRightAnimationAsset(TEXT("/Game/Boss/Sprites/WalkRight"))
			, AttackAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/Attack.Attack"))
			, IdleAnimationAsset(TEXT("/Game/Boss/Sprites/idle"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	RootComponent->RelativeRotation = FRotator(0.0f, 0.0f, 90.0f);

	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollision"));
	MeleeCollisionComp->AttachParent = GetCapsuleComponent();

	MeleeDamage = 25.0f;
	currentEgg = 1;
	NumberEggs = 1;
	MeleeStrikeCooldown = 1.0f;
}

void ASpiderBoss::BeginPlay()
{
	Super::BeginPlay();

	if (MeleeCollisionComp)
	{
		MeleeCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASpiderBoss::OnMeleeCompBeginOverlap);
	}

	bIsPlacingEggs = false;
	bFoundEggPlace = false;

	HomeLocation = GetActorLocation();

	ASpiderBossAIController* AIController = Cast<ASpiderBossAIController>(GetController());
	if (AIController)
	{
		AIController->SetTargetToFollow(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}

<<<<<<< HEAD
	MeleeDamage = 24.0f;
	MeleeStrikeCooldown = 1.0f;
	*/
=======
void ASpiderBoss::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();
	long TravelDirectionX = PlayerVelocity.X;
	long TravelDirectionY = PlayerVelocity.Y;
	int DesiredSpriteRotation = 0;

	if (bIsAttacking)
	{
		GetSprite()->SetFlipbook(AttackAnimation);
		return;
	}

	if (PlayerSpeed == 0.0f)
	{
		GetSprite()->SetFlipbook(IdleAnimation);
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
		TravelDirectionY < 0.0f ? GetSprite()->SetFlipbook(WalkUpAnimation) : GetSprite()->SetFlipbook(WalkDownAnimation);
		break;

	default:
		break;
	}
}

void ASpiderBoss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCharacter();
	DefineDifficulty();
	isAtEggLocation(currentEgg);
}

void ASpiderBoss::isAtEggLocation(int currentEgg)
{
	if (bIsPlacingEggs)
	{
		/*switch (currentEgg)
		{
		case 1:
			if ((GetActorLocation() - HomeLocation).Size() < 50.0f)
			{
				SpawnEgg();
				PlaceEggs(currentEgg);
			}
			break;

		case 2:
			if ((GetActorLocation() - EggLocation).Size() < 50.0f)
			{
				SpawnEgg();
				PlaceEggs(currentEgg);
			}
			break;

		case 3:
			if ((GetActorLocation() - EggLocation2).Size() < 50.0f)
			{
				SpawnEgg();
				PlaceEggs(currentEgg);
			}
			break;

		case 4:
			if ((GetActorLocation() - EggLocation3).Size() < 50.0f)
			{
				SpawnEgg();
				PlaceEggs(currentEgg);
			}
			break;

		case 5:
			if ((GetActorLocation() - EggLocation4).Size() < 50.0f)
			{
				SpawnEgg();
				PlaceEggs(currentEgg);
			}
			break;

		default:
			break;
		}*/

		if ((GetActorLocation() - HomeLocation).Size() < 50.0f && !bFoundEggPlace)
		{
			bFoundEggPlace = true;
			SpawnEgg();
			PlaceEggs(currentEgg);	
		}
	}
}

void ASpiderBoss::SpawnEgg()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Placed Egg!"));
	currentEgg++;
}

void ASpiderBoss::DefineDifficulty()
{
	if (Health == 300.0f && bIsPlacingEggs == false && NumberEggs != 2)
	{
		NumberEggs = 2;
		GetCharacterMovement()->MaxWalkSpeed += 20.0f;
	}

	if (Health == 200.0f && bIsPlacingEggs == false && NumberEggs != 3)
	{
		NumberEggs = 3;
		GetCharacterMovement()->MaxWalkSpeed += 20.0f;
	}

	if (Health == 100.0f && bIsPlacingEggs == false && NumberEggs != 5)
	{
		NumberEggs = 5;
		GetCharacterMovement()->MaxWalkSpeed += 60.0f;
	}

	if (NumberEggs > 1 && bIsPlacingEggs == false)
	{
		bIsPlacingEggs = true;
		GetCharacterMovement()->MaxWalkSpeed += 100.0f;
		PlaceEggs(currentEgg);
	}
}

void ASpiderBoss::PlaceEggs(int currentEgg)
{
	ASpiderBossAIController* AIController = Cast<ASpiderBossAIController>(GetController());

	if (currentEgg > NumberEggs)
	{
		GetCharacterMovement()->MaxWalkSpeed -= 100.0f;
		AIController->SetTargetToFollow(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		this->currentEgg = 1;
		bIsPlacingEggs = false;

		return;
	}

	AIController->SetTargetToFollow(nullptr);

	switch (currentEgg)
	{
	case 1:
		AIController->SetNextWaypoint(HomeLocation);
		bFoundEggPlace = false;
		break;

	case 2:
		HomeLocation.Y -= 300.0f;
		AIController->SetNextWaypoint(HomeLocation);
		bFoundEggPlace = false;
		break;

	case 3:
		HomeLocation.Y += 150.0f;
		AIController->SetNextWaypoint(HomeLocation);
		bFoundEggPlace = false;
		break;

	case 4:
		HomeLocation.X -= 200.0f;
		AIController->SetNextWaypoint(HomeLocation);
		bFoundEggPlace = false;
		break;

	case 5:
		HomeLocation.X += 100.0f;
		AIController->SetNextWaypoint(HomeLocation);
		bFoundEggPlace = false;
		break;

	default:
		break;
	}
}

void ASpiderBoss::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();
}

float ASpiderBoss::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		return 0.f;
	}

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;

		if (Health <= 0)
		{
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}
	}

	return ActualDamage;
}

void ASpiderBoss::PerformMeleeStrike(AActor* HitActor)
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

void ASpiderBoss::OnMeleeCompBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	/* Stop any running attack timers */
	TimerHandle_MeleeAttack.Invalidate();

	PerformMeleeStrike(OtherActor);

	/* Set re-trigger timer to re-check overlapping pawns at melee attack rate interval */
	GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &ASpiderBoss::OnRetriggerMeleeStrike, MeleeStrikeCooldown, true);
}

void ASpiderBoss::OnRetriggerMeleeStrike()
{
	/* Apply damage to a single random pawn in range. */
	TArray<AActor*> Overlaps;
	MeleeCollisionComp->GetOverlappingActors(Overlaps, ABaseCharacter::StaticClass());
	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		AHunter* OverlappingPawn = Cast<AHunter>(Overlaps[i]);
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
>>>>>>> origin/master
}


