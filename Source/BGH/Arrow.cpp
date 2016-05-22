// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "Arrow.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "EnemySpider.h"
#include "Hunter.h"


// Sets default values
AArrow::AArrow()
{

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> ArrowSpriteAsset;
		FConstructorStatics() :
			ArrowSpriteAsset(TEXT("/Game/Player_Content/Player_Sprites/download_arrow_Sprite_0"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetNotifyRigidBodyCollision(true);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnCompBeginOverlap);		// set up a notification for when this component hits something blocking

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed = 750.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	//Arrow Component
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->AttachTo(RootComponent);

	// Arrow Sprite
	ArrowSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ArrowSprite"));
	ArrowSprite->SetSprite(ConstructorStatics.ArrowSpriteAsset.Get());
	ArrowSprite->RelativeRotation = FRotator(0.0f, 180.0f, 90.0f);
	ArrowSprite->RelativeLocation = FVector(-10.0f, 0.0f, 0.0f);
	ArrowSprite->AttachTo(RootComponent);

	// Die after 6 seconds by default
	InitialLifeSpan = 2.0f;

	Damage = 25.0f;

}

void AArrow::OnCompBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AEnemySpider* SpiderlingHit = Cast<AEnemySpider>(OtherActor);
	//ASpiderBoss* SpiderBossHit = Cast<ASpiderBoss>(OtherActor);

	if (SpiderlingHit)
	{
		Destroy();

		FPointDamageEvent DmgEvent;
		DmgEvent.Damage = Damage;

		OtherActor->TakeDamage(DmgEvent.Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
	}
}
