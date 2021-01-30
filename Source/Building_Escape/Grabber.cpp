// Copyright Stewart Mkcenzie 2021
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

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
	FindPhysicsHandle();
	SetUpInputComponent();
}

void UGrabber::SetUpInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandle() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("%s does not have a physics handle component. Attach one to avoid any errors"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	// try and reach actors with physics body collison channel
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	//if we hit something attach the physics body
	if (HitResult.GetActor()) {
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayersReach());
	}
}

FVector UGrabber::GetPlayersReach() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return  PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));
	//remove/relase the physics handle
	if (PhysicsHandle->GetGrabbedComponent()) {
		PhysicsHandle->ReleaseComponent();
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//physical handle has been attached is attached

	if (PhysicsHandle->GrabbedComponent) {
		//move object we are holding

		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}

	//move object we are holding
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {
	//Draw a line from player showing the reach

	DrawDebugLine(GetWorld(), GetPlayersWorldPos(), GetPlayersReach(), FColor(0, 255, 0), false, 0.f, 0, 5.f);

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetPlayersWorldPos(), GetPlayersReach(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	//AActor* ActorHit = Hit.GetActor();

	//if (ActorHit)
	//	UE_LOG(LogTemp, Warning, TEXT("%s was hit."), *ActorHit->GetName());

	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}