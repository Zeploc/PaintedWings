// Fill out your copyright notice in the Description page of Project Settings.

#include "Collectable.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/BirdPlayer.h"
#include "Player/BirdController.h"
#include "Engine/World.h"

// Sets default values
ACollectable::ACollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Colission Sphere"));
	//SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::OnCompOverlap);
	RootComponent = SphereCollision;
	CollectableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collectable Mesh"));
	OnDestroyed.AddDynamic(this, &ACollectable::OnCollectableDestroyed);
	CollectableMesh->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ACollectable::OnCompOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//	ABirdController* ControllerRef = Cast<ABirdController>(GetWorld()->GetFirstPlayerController());
//	if (ControllerRef && CollectableMesh->IsVisible() && Cast<ABirdPlayer>(OtherActor))
//	{
//		//ControllerRef->AddCollectablePoint(this);
//		//CollectableMesh->SetVisibility(false);
//	}
//}

void ACollectable::OnCollectableDestroyed(AActor* Act)
{
	
}

void ACollectable::RespawnCollectable()
{
	CollectableMesh->SetVisibility(true);
}

