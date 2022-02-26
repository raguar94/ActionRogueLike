// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARLInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ARLAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ActionComponent/ARLActionComponent.h"

// Sets default values
AARLCharacter::AARLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComponent = CreateDefaultSubobject<UARLInteractionComponent>("Interaction Component");

	AttributeComp = CreateDefaultSubobject<UARLAttributeComponent>("Attribute Component");

	ActionComp = CreateDefaultSubobject<UARLActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
}


void AARLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AARLCharacter::OnHealthChanged);
}

FVector AARLCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called when the game starts or when spawned
void AARLCharacter::BeginPlay()
{
	Super::BeginPlay();
	 
}

// Called every frame
void AARLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void AARLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AARLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AARLCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AARLCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Blackhole", IE_Pressed, this, &AARLCharacter::ShootBlackhole);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &AARLCharacter::Teleport);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AARLCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AARLCharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AARLCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AARLCharacter::SprintStop);
}


void AARLCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void AARLCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AARLCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AARLCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AARLCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}


void AARLCharacter::ShootBlackhole()
{
	ActionComp->StartActionByName(this, "Blackhole");
}

void AARLCharacter::Teleport()
{
	ActionComp->StartActionByName(this, "Dash");
}

void AARLCharacter::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// Rage added equal to damage received (Abs to turn into positive rage number)
		const float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);
	}

	// Died
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void AARLCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

void AARLCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

FTransform AARLCharacter::GetProjectileSpawnTransform()
{
	FHitResult HitResult;
	const FVector CameraLocation = CameraComp->GetComponentLocation();
	const FVector EndPoint = CameraLocation + CameraComp->GetForwardVector() * 1500.0f;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	bool bHitSuccesfull = GetWorld()->LineTraceSingleByObjectType(HitResult, CameraLocation, EndPoint, ObjectQueryParams);

	const FVector TargetLocation = bHitSuccesfull ? HitResult.ImpactPoint : EndPoint;

	const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetLocation);

	return FTransform(SpawnRotation, HandLocation);
}

