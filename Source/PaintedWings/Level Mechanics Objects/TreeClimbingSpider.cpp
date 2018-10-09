// Fill out your copyright notice in the Description page of Project Settings.

#include "TreeClimbingSpider.h"
#include "Components/BoxComponent.h"
#include "PaintedWings/Player/BirdPlayer.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "PaintedWings/Player/BirdPlayer.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
ATreeClimbingSpider::ATreeClimbingSpider()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Box");
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATreeClimbingSpider::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATreeClimbingSpider::OnOverlapEnd);

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(100.0f, 100.0f, 100.0f); // ...at this rotation rate
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
}

// Called when the game starts or when spawned
void ATreeClimbingSpider::BeginPlay()
{
	Super::BeginPlay();
	vOriginalLocation = this->GetActorLocation();
}

// Called every frame
void ATreeClimbingSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerRef)
	{
		FVector Direction = PlayerRef->GetActorLocation() - this->GetActorLocation();
		Direction.Normalize();
		AddMovementInput(Direction, 1.0);
	}
	else
	{
		TSubclassOf<ABirdPlayer> ClassToFind;
		ClassToFind = ABirdPlayer::StaticClass();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, ActorArray);
		UE_LOG(LogTemp, Warning, TEXT("Num %d"), ActorArray.Num());
		for (int i = 0; i < ActorArray.Num(); i++)
		{
			if (ABirdPlayer* BirdRef = Cast<ABirdPlayer>(ActorArray[i]))
			{
				UE_LOG(LogTemp, Warning, TEXT("FOLLOW"));
				PlayerRef = BirdRef;
			}
		}
	}

}

// Called to bind functionality to input
void ATreeClimbingSpider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATreeClimbingSpider::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef)
	{
		BirdRef->Death();
		this->SetActorLocation(vOriginalLocation);
	}
}

void ATreeClimbingSpider::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef)
	{
		
	}
}

