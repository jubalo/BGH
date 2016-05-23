// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "PaperSpriteComponent.h"
#include "Arrow_Projectile.h"
#include "Arrow.h"
#include "PaperFlipbookComponent.h"
#include "Hunter.h"
#include "Engine.h"
#include "BaseCharacter.h"


// Sets default values
AHunter::AHunter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> AttackDownAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> AttackUpAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> AttackLeftAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> AttackRightAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> BowLeftAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> BowRightAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> BowUpAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> BowDownAnimationAsset;
		FConstructorStatics():
			IdleDownAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Idle_Down")),
			IdleUpAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Idle_Up")),
			IdleLeftAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Idle_Left")),
			IdleRightAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Idle_Right")),
			LeftSideRunningAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Walk_Side_Left")),
			RightSideRunningAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Walk_Side_Right")),
			UpRunningAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Walk_Up")),
			DownRunningAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Walk_Down")),
			AttackDownAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Sword_Attack_Down")),
			AttackUpAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Sword_Attack_Up")),
			AttackLeftAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Sword_Attack_Left")),
			AttackRightAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Sword_Attack_Right")),
			BowLeftAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Bow_Attack_Left")),
			BowRightAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Bow_Attack_Right")),
			BowUpAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Bow_Attack_Up")),
			BowDownAnimationAsset(TEXT("/Game/Player_Content/Player_Sprites/Player_FB/Bow_Attack_Down"))
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
	AttackDownAnimation = ConstructorStatics.AttackDownAnimationAsset.Get();
	AttackUpAnimation = ConstructorStatics.AttackUpAnimationAsset.Get();
	AttackLeftAnimation = ConstructorStatics.AttackLeftAnimationAsset.Get();
	AttackRightAnimation = ConstructorStatics.AttackRightAnimationAsset.Get();
	BowLeftAnimation = ConstructorStatics.BowLeftAnimationAsset.Get();
	BowRightAnimation = ConstructorStatics.BowRightAnimationAsset.Get();
	BowUpAnimation = ConstructorStatics.BowUpAnimationAsset.Get();
	BowDownAnimation = ConstructorStatics.BowDownAnimationAsset.Get();

	GetSprite()->SetFlipbook(IdleDownAnimation);
	GetSprite()->RelativeLocation = FVector(0.0f, 0.0f, -25.0f);
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HunterBase"));
	}

	// Set the size of our collision capsule.
	GetCapsuleComponent()->bGenerateOverlapEvents = true;
	GetCapsuleComponent()->SetCapsuleHalfHeight(29.0f);
	GetCapsuleComponent()->SetCapsuleRadius(14.0f);
	GetCapsuleComponent()->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);

	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollision"));
	MeleeCollisionComp->SetCapsuleHalfHeight(30.0f);
	MeleeCollisionComp->SetCapsuleRadius(30.0f);
	MeleeCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeleeCollisionComp->AttachParent = GetCapsuleComponent();

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	// Y X Z
	CameraBoom->RelativeRotation = FRotator(-90.0f, -90.0f, 0.0f);
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

	MeleeDamage = 50;
	Orientation = 3;
	bAttacking = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	NoiseMaker = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));
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
	// Apply the input to the character motion
	if (!(bAttacking || bLoadingBow))
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}

	MakeNoise(1.0, this, GetActorLocation());
}

void AHunter::VerticalMove(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	if (!(bAttacking || bLoadingBow))
	{
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
	}

	MakeNoise(1.0, this, GetActorLocation());
}

// Called to bind functionality to input
void AHunter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	InputComponent->BindAction("Sword_Attack", IE_Pressed, this, &AHunter::BeginSwordAttack);
	InputComponent->BindAction("Sword_Attack", IE_Released, this, &AHunter::QueueStopAttack);

	InputComponent->BindAction("Bow_Attack", IE_Pressed, this, &AHunter::BeginLoadingBow);
	InputComponent->BindAction("Bow_Attack", IE_Released, this, &AHunter::StopLoadingBow);

	InputComponent->BindAxis("Side_Running", this, &AHunter::HorizontalMove);
	InputComponent->BindAxis("Vertical_Running", this, &AHunter::VerticalMove);

}

void AHunter::UpdateCharacter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HP: %f"), Health));
	// Update animation to match the motion
	UpdateAnimation();
}

void AHunter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	
	bool PlayerOrientation = true;
	UPaperFlipbook* DesiredAnimation = IdleDownAnimation;

	// Are we moving horizontally?
	if (PlayerVelocity.X != 0.0f && abs((long) PlayerVelocity.X) >= abs((long) PlayerVelocity.Y)) {
		PlayerOrientation = true;
		
		//Are we moving left?
		if (PlayerVelocity.X < 0.0f) {
			Orientation = 0;
			DesiredAnimation = LeftSideRunningAnimation;
			MeleeCollisionComp->SetRelativeLocation(FVector(-20.0f, 0.0f, 0.0f));
		}
		else {
			Orientation = 2;
			DesiredAnimation = RightSideRunningAnimation;
			MeleeCollisionComp->SetRelativeLocation(FVector(20.0f, 0.0f, 0.0f));
		}
	}

	// Are we moving Vertically?
	else if (PlayerVelocity.Y != 0.0f) {
		PlayerOrientation = false;
		
		// Are we moving Down?
		if (PlayerVelocity.Y < 0.0f) {
			Orientation = 1;
			DesiredAnimation = UpRunningAnimation;
			MeleeCollisionComp->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
		}
		else {
			Orientation = 3;
			DesiredAnimation = DownRunningAnimation;
			MeleeCollisionComp->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
		}
	}

	// Are we attacking?
	else if (bAttacking) {
		if (Orientation == 0) {
			DesiredAnimation = AttackLeftAnimation;
		}
		else if (Orientation == 2) {
			DesiredAnimation = AttackRightAnimation;
		}
		else if (Orientation == 1) {
			DesiredAnimation = AttackUpAnimation;
		}
		else {
			DesiredAnimation = AttackDownAnimation;
		}
	}

	// Are we loading a bow?
	else if (bLoadingBow) {
		if (Orientation == 0) {
			DesiredAnimation = BowLeftAnimation;
		}
		else if (Orientation == 2) {
			DesiredAnimation = BowRightAnimation;
		}
		else if (Orientation == 1) {
			DesiredAnimation = BowUpAnimation;
		}
		else {
			DesiredAnimation = BowDownAnimation;
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

	
}


//sword stuff
void AHunter::BeginSwordAttack() {
	bAttacking = true;
	bWantsToAttack = true;

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandler, this, &AHunter::StopSwordAttack, 0.4f, false);
}

void AHunter::PerformMeleeStrike(AActor* HitActor)
{
	if (HitActor && HitActor != this && IsAlive())
	{
		ABaseCharacter* OtherPawn = Cast<ABaseCharacter>(HitActor);
		if (OtherPawn)
		{
			FPointDamageEvent DmgEvent;
			DmgEvent.Damage = MeleeDamage;

			HitActor->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);
		}
	}
}

void AHunter::QueueStopAttack() 
{
	bWantsToAttack = false;
}

void AHunter::StopSwordAttack() 
{
	TArray<AActor*> Overlaps;
	MeleeCollisionComp->GetOverlappingActors(Overlaps, ABaseCharacter::StaticClass());
	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		ABaseCharacter* OverlappingPawn = Cast<ABaseCharacter>(Overlaps[i]);
		if (OverlappingPawn)
		{
			PerformMeleeStrike(OverlappingPawn);
		}
	}

	if (!bWantsToAttack) {
		bAttacking = false;
	}
	else {
		BeginSwordAttack();
	}
}


// bow stuff
void AHunter::BeginLoadingBow()
{
	bLoadingBow = true;
	GetWorld()->GetTimerManager().SetTimer(BowTimerHandler, this, &AHunter::ShootArrow, 0.4, false);
}

void AHunter::StopLoadingBow()
{
	bLoadingBow = false;
}


void AHunter::ShootArrow()
{
	UWorld* const World = GetWorld();
	FVector  SpawnLocation = GetActorLocation();
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	if (World != NULL)
	{
		if (Orientation == 2) {			// Right
			//Rotation.Yaw = 90.0f;
			SpawnLocation.X += 20.0f;
			SpawnLocation.Y -= 10.0f;
		}
		else if (Orientation == 3) {	// Down
			Rotation.Yaw = 90.0f;
			//SpawnLocation.X +=
			SpawnLocation.Y += 15.0f;
		}
		else if (Orientation == 0) {		// Left
			Rotation.Yaw = 180.0f;
			SpawnLocation.X -= 20.0f;
			SpawnLocation.Y -= 10.0f;
		}
		else {
			Rotation.Yaw = -90.0f;
			SpawnLocation.Y -= 40.0f;
		}

		if (bLoadingBow) {
			// spawn the projectile 
			World->SpawnActor<AArrow>(AArrow::StaticClass(), SpawnLocation, Rotation);
		}
	}

	StopLoadingBow();
}

float AHunter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
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
