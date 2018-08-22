// Fill out your copyright notice in the Description page of Project Settings.

#include "DashWall.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/BirdPlayer.h"
#include "Player/BirdController.h"
#include "Components/BoxComponent.h"

#include "Engine.h"

// Sets default values
ADashWall::ADashWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Web = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Web Mesh"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Web Collision"));
	SetRootComponent(Web);


	//Web->OnComponentHit.AddDynamic(this, &ADashWall::OnCompHit);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADashWall::OnCompOverlap);
}

// Called when the game starts or when spawned
void ADashWall::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerControllerRef = Cast<ABirdController>(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void ADashWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	if (bBroken)
	{
		DeathSequence(DeltaTime);
	}
	else
	{
		ABirdPlayer* PlayerRef = Cast<ABirdPlayer>(PlayerControllerRef->GetCharacter());
		FVector XYSpeed = PlayerRef->GetVelocity();
		XYSpeed.Y = 0.0f;
		if (FVector::Distance(PlayerRef->GetActorLocation(), GetActorLocation()) < 500.0f && PlayerRef->IsDashing && XYSpeed.Size() > MinimumSpeed)
		{
			CollisionBox->SetCollisionProfileName("OverlapAll");
		}
		else
		{
			if (CollisionBox->GetCollisionProfileName() != "BlockAll") CollisionBox->SetCollisionProfileName("BlockAll");
		}
	}
}

void ADashWall::DeathSequence(float Timepased)
{
	timer += Timepased;
	if (timer > DestroyTime)
	{
		Destroy();
	}
}

void ADashWall::OnCompOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABirdPlayer* BirdPlayerRef = Cast<ABirdPlayer>(OtherActor);
	if (BirdPlayerRef)
	{
		if (BirdPlayerRef->IsDashing)
		{
			bBroken = true;
			CollisionBox->SetCollisionProfileName("OverlapAll");
		}
	}
}



