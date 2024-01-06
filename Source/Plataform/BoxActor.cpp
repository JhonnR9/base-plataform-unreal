// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxActor.h"

// Sets default values
ABoxActor::ABoxActor()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite Box"));

	RootComponent = Collider;
	Sprite->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ABoxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoxActor::NotifyHit(UPrimitiveComponent* MyComp, 
	AActor* Other, UPrimitiveComponent* OtherComp, 
	bool bSelfMoved, 
	FVector HitLocation, 
	FVector HitNormal, 
	FVector NormalImpulse, 
	const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Colisão Iniciada com %s!"), *Other->GetName());
}

void ABoxActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Log, TEXT("Sobreposiçao Iniciada com %s!"), *OtherActor->GetName());
}

