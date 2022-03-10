// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	//check whether physics handle is available
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has not physics handle attached!"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found on %s"), *GetOwner()->GetName());
		InputComponent->BindAction(FName(TEXT("Grab")), EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction(FName(TEXT("Grab")), EInputEvent::IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab button is pressed"));
	GetFirstPhysicsBodyInReach();
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab button is released"));
}

AActor* UGrabber::GetFirstPhysicsBodyInReach() const
{
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerLocation;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	FVector End = PlayerLocation + Reach * PlayerRotation.Vector();
	// UE_LOG(LogTemp, Warning, TEXT("%s, %s"), *PlayerLocation.ToString(), *PlayerRotation.ToString());
	// DrawDebugLine(GetWorld(), PlayerLocation, End, FColor(0xffff00), false, 0.f, 0U, 2.f);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("Test Tag")), false, GetOwner());
	bool bHitSomeThing = GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		PlayerLocation,
		End,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		// FCollisionQueryParams::DefaultQueryParam
		TraceParams);

	if (bHitSomeThing)
	{
		AActor *HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
		return HitActor;
	}

	return nullptr;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//when physics handle is attached, move with me	
}
