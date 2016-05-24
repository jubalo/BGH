// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "SpiderBoss.generated.h"

/**
*
*/
UCLASS(config = Game)
class BGH_API ASpiderBoss : public ABaseCharacter
{
	GENERATED_BODY()

	ASpiderBoss(const FObjectInitializer& ObjectInitializer);
	/*
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UCapsuleComponent* MeleeCollisionComp;

	float LastMeleeAttackTime;

	FVector HomeLocation;

	FVector EggLocation;

	FVector EggLocation2;

	FVector EggLocation3;

	FVector EggLocation4;

	// Booleans
	bool bIsAttacking;

	bool bIsPlacingEggs;

	bool bFoundEggPlace;

protected:

	// The animations to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkUpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkDownAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkLeftAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkRightAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// The animation to play while attacking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackAnimation;

	UFUNCTION(Category = "Attacking")
		void PerformMeleeStrike(AActor* HitActor);

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
		float MeleeDamage;

	float MeleeStrikeCooldown;

	int NumberEggs;

	UFUNCTION()
		void OnMeleeCompBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void OnRetriggerMeleeStrike();

	void UpdateCharacter();

	void UpdateAnimation();

	void DefineDifficulty();

	void PlaceEggs(int currentEgg);

	void SpawnEgg();

	int currentEgg;

	void isAtEggLocation(int currentEgg);

	FTimerHandle TimerHandle_MeleeAttack;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;
	*/
};
