// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "Arrow.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


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
	CollisionComp->InitSphereRadius(3.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AArrow::OnHit);		// set up a notification for when this component hits something blocking

																								// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed = 500.f;
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
	InitialLifeSpan = 6.0f;

}


void AArrow::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		if (OtherActor->bCanBeDamaged) {
			// damage
		}

		Destroy();
	}
}
