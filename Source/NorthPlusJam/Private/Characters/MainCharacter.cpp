// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

#include "Characters/MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create a spring arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	//SpringArmComponent->SetupAttachment(GetMesh(), "head");
	SpringArmComponent->TargetArmLength = 700.f;
	//SpringArmComponent->AddLocalRotation(FRotator(0, 90.f, 0));
	SpringArmComponent->bUsePawnControlRotation = true;

	//Create Camera 
	MainCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Top Camera Component"));
	MainCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	MainCameraComponent->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	//ULocalPlayer* Player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//ULocalPlayer* Player = GetWorld()->GetFirstLocalPlayerFromController();
	//ULocalPlayer* Player = GetWorld()->GetFirstLocalPlayerFromController();
	/*if (ULocalPlayer* PlayerController = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = PlayerController->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), Priority);
			}
		}
	}*/

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			InputSystem->AddMappingContext(InputMapping, 0);
		}
	}


}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float mouseX;
	float mouseY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouseX, mouseY);
	//UE_LOG(MyLog, Warning, TEXT("Mouse Location: %f, %f"), mouseX, mouseY);

	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	AController* PlayerController = Controller;
	// --------------------
	FHitResult OutHit;
	//FVector Start = MainCameraComponent->GetComponentLocation();
	FVector Start = myCharacter->GetActorLocation();
	FVector ForwardVector = MainCameraComponent->GetForwardVector();
	Start = Start + (ForwardVector * SpringArmComponent->TargetArmLength);
	FVector End = Start + (ForwardVector * 5000.f);
	//
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
	FVector Location, Direction;
	Cast<APlayerController>(GetController())->DeprojectMousePositionToWorld(Location, Direction);
	//DrawDebugLine(GetWorld(), Location, Location + Direction * 100.0f, FColor::Red, true);

	//-----------------

	FVector WorldLocation;
	FVector WorldDirection;
	float DistanceAboveGround = 200.0f;

	Cast<APlayerController>(GetController())->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FVector PlaneOrigin(0.0f, 0.0f, DistanceAboveGround);

	FVector ActorWorldLocation = FMath::LinePlaneIntersection(
		WorldLocation,
		WorldLocation + WorldDirection,
		PlaneOrigin,
		FVector::UpVector);


	//-----------------

	//FVector worldLoc, worldDir;
	//myController->DeprojectMousePositionToWorld(worldLoc, worldDir);
	//FString testString = "loc " + worldLoc.ToString() + " dir " + worldDir.ToString();
	

	DrawDebugLine(GetWorld(), Location, ActorWorldLocation, FColor::Red, true);
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, ActorWorldLocation.ToString());










	FRotator temp = myCharacter->GetActorRotation();
	const FRotator resultRotation = FMath::RInterpTo(temp, temp + FRotator(0, 10, 0), DeltaTime, 2.0);


	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, testString);






	/*UCapsuleComponent Capsule = myCharacter->GetCapsuleComponent();
	*/
	//Capsule->SetWorldLocation(ActorWorldLocation);
	//myCharacter->SetActorLocationAndRotation(myCharacter->GetActorLocation(), WorldDirection, false, 0, ETeleportType::None);

	myCharacter->SetActorRotation(resultRotation);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
	}
	

}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2D>();
	
	/*
	const FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, MovementVector.Y);
	const FVector Right = GetActorRightVector();
	AddMovementInput(Right, MovementVector.X);
	*/

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);


}
