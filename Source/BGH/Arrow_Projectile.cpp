// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Arrow_Projectile.h"


// Sets default values
AArrow_Projectile::AArrow_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> ArrowSpriteAsset;
		FConstructorStatics() :
			ArrowSpriteAsset(TEXT("/Game/Player_Content/Player_Sprites/download_arrow_Sprite_0"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowRoot"));
	}

/**/	
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->RelativeRotation = FRotator(90.0f, 0.0f, 90.0f);
	CollisionComp->SetCapsuleHalfHeight(13.0f);
	CollisionComp->SetCapsuleRadius(3.0f);
	CollisionComp->OnComponentHit.AddDynamic(this, &AArrow_Projectile::OnHit);		// set up a notification for when this component hits something blocking
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;
/**/
	//Arrow Component
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->RelativeRotation = FRotator(-90.0f, 0.0f, 0.0f);
	ArrowComp->AttachTo(RootComponent);

	// Arrow Sprite
	ArrowSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ArrowSprite"));
	ArrowSprite->SetSprite(ConstructorStatics.ArrowSpriteAsset.Get());
	ArrowSprite->RelativeRotation = FRotator(90.0f, 90.0f, 0.0f);
	ArrowSprite->AttachTo(RootComponent);
/**/
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = RootComponent;
/** /
	ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed = 500.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 1.0f; // No gravity
/**/

	// Die after 3 seconds by default
	InitialLifeSpan = 60.0f;
}

// Called when the game starts or when spawned
void AArrow_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow_Projectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	UpdateMovement();
}

void AArrow_Projectile::UpdateMovement() {

	

}

void AArrow_Projectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("hit"));
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		Destroy();
	}
}

