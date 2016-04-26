// Fill out your copyright notice in the Description page of Project Settings.

#include "BGH.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "TreasureChest.h"


// Sets default values
ATreasureChest::ATreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> OpenTreasureAnimationAsset;
		
		FConstructorStatics() :
			OpenTreasureAnimationAsset(TEXT("/Game/Miscellaneous/FB_Open_Treasure"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TreasureRoot"));
	}

	/** /
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->bGenerateOverlapEvents = true;
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATreasureChest::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATreasureChest::OnOverlapEnd);
	RootComponent = TriggerBox;
	/**/
	
	OpenTreasureFB = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("OpenTreasureFB"));
	OpenTreasureFB->SetFlipbook(ConstructorStatics.OpenTreasureAnimationAsset.Get());
	OpenTreasureFB->SetLooping(false);
	OpenTreasureFB->Stop();
	OpenTreasureFB->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	OpenTreasureFB->RelativeRotation = FRotator(0.0f, 0.0f, 270.0f);
	OpenTreasureFB->CanCharacterStepUpOn = ECB_No;
	OpenTreasureFB->AttachTo(RootComponent);

	bIsOpen = false;
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreasureChest::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATreasureChest::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTemp, Warning, TEXT("begin"));
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
	}
}

void ATreasureChest::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	UE_LOG(LogTemp, Warning, TEXT("end"));
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
	}
}

void ATreasureChest::SetupPlayerInputComponent(UInputComponent * InputComponent)
{
	InputComponent->BindAction("Interact", IE_Pressed, this, &ATreasureChest::Open);
}

void ATreasureChest::Open()
{

	UE_LOG(LogTemp, Warning, TEXT("open"));
	bIsOpen = true;
	OpenTreasureFB->Play();
}

