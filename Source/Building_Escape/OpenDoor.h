// Copyright Stewart Mkcenzie 2021

#pragma once
#include "Engine/TriggerVolume.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void HandleDoor(float DeltaTime, float& Angle, float& RoationSpeed);

private:

	float InitialYaw;
	float CurrentYaw;
	float DoorLastOpened = 0.f;
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate;
	UPROPERTY(EditAnywhere)
		float DoorCloseDelay = .5f;
	UPROPERTY(EditAnywhere)
		float CloseSpeed = 4.f;
	UPROPERTY(EditAnywhere)
		float OpenSpeed = 4.f;
	UPROPERTY(EditAnywhere)
		float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
		AActor* ActorThatOpens;
};
