// Fill out your copyright notice in the Description page of Project Settings.

#include "TreeClimbingSpider.h"
#include "Components/BoxComponent.h"
#include "PaintedWings/Player/BirdPlayer.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaintedWings/Player/BirdController.h"


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
	bCanKill = true;
	fDefaultSpeed = GetCharacterMovement()->MaxFlySpeed;
}

// Called every frame
void ATreeClimbingSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerRef != nullptr)
	{
		if (PlayerRef->bRespawning) { UE_LOG(LogTemp, Warning, TEXT("Respawning")); return; };

		FVector StartLocation = LogCenter->GetActorLocation();
		StartLocation.Z = PlayerRef->GetActorLocation().Z;
		FVector DirectionToWallBelowPlayer = PlayerRef->GetActorLocation() - StartLocation;
		DirectionToWallBelowPlayer.Normalize();
		FVector StartTraceLocation = LogCenter->GetActorLocation();
		StartTraceLocation.Z = GetActorLocation().Z;
		FVector EndTraceLocation = StartTraceLocation + DirectionToWallBelowPlayer * 2000.0f;
				
		
		FHitResult LogHitResult;
		if (GetWorld()->LineTraceSingleByChannel(LogHitResult, StartTraceLocation, EndTraceLocation, ECC_GameTraceChannel2))
		{
			Speed = GetCharacterMovement()->GetMaxSpeed() + (GetActorLocation() - LogHitResult.Location).Size();
			SetActorLocation(LogHitResult.Location);
			SetActorRotation(DirectionToWallBelowPlayer.Rotation());
		}


		AddMovementInput(FVector(0, 0, 1), 1.0);
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
				if (BirdRef->bRespawning) { return; };
				if (ABirdController* controller = Cast<ABirdController>(BirdRef->GetController()))
				{
					if (controller->GetCheckpoint())
					{
						FVector Location = controller->GetCheckpoint()->GetActorLocation();
						Location.Z -= SpiderSpawnDistance;
						this->SetActorLocation(Location);
						bCanKill = true;
					}
					else
					{
						//UE_LOG(LogTemp, Warning, TEXT("FOLLOW"));
						PlayerRef = BirdRef;
						FVector Location = PlayerRef->GetActorLocation();
						Location.Z -= SpiderSpawnDistance + 200.0f;
						this->SetActorLocation(Location);
						bCanKill = true;
					}
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("FOLLOW"));
					PlayerRef = BirdRef;
					FVector Location = PlayerRef->GetActorLocation();
					Location.Z -= SpiderSpawnDistance + 200.0f;
					this->SetActorLocation(Location);
					bCanKill = true;
				}
			}
		}
	}

}

// Called to bind functionality to input
void ATreeClimbingSpider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATreeClimbingSpider::SetMovementSpeed(float _speed)
{
	GetCharacterMovement()->MaxFlySpeed = _speed;
}

void ATreeClimbingSpider::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef && !BirdRef->bRespawning && bCanKill)
	{
		BirdRef->Death();
		PlayerRef = nullptr;
		bCanKill = false;
	}
}

void ATreeClimbingSpider::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ABirdPlayer* BirdRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdRef)
	{
		PlayerRef = nullptr;
	}
}

