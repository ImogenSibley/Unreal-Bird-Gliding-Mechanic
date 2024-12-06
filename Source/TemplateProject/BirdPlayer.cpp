// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Animation/AnimInstance.h"

// Sets default values
ABirdPlayer::ABirdPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsSprinting = false; 
	bIsGrounded = true;
	bIsJumping = false;
	bIsGliding = false;
	bIsFalling = false;
	bIsIdle = true;

	//create a Spring Arm to attach the Camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f; //set how far the camera will be from the bird
	SpringArm->bUsePawnControlRotation = true; //allow camera to rotate with the player
	SpringArm->bEnableCameraLag = true; //smooth camera follow
	SpringArm->CameraLagSpeed = 3.0f; //control camera lag speed

	//create a Camera and attach it to the Spring Arm
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; //camera shouldn't inherit rotation from the player

}

// Called when the game starts or when spawned
void ABirdPlayer::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Bird Player Spawned"));
	
}

// Called every frame
void ABirdPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//check if player is falling or grounded
	bIsFalling = GetCharacterMovement()->IsFalling();
	bIsGrounded = !bIsFalling;
	bIsJumping = !bIsGrounded && !bIsGliding;

	//update idle state based on movement
	bIsIdle = (GetVelocity().Size() <= 0.1f && bIsGrounded); //player idle if not moving and grounded

	UpdateAnimations();

}

void ABirdPlayer::MoveForward(float Value)
{
	if (Value != 0.0f) //if movement isnt zero
	{
		//add movement in the forward direction
		AddMovementInput(GetActorForwardVector(), Value);
		bIsIdle = false; //if moving forward, player not idle
	}
	else
	{
		//no forward movement means player might be idle
		if (!bIsGrounded) //if not grounded, player not idle
			bIsIdle = false;
		else
			bIsIdle = true;
	}
}

void ABirdPlayer::MoveRight(float Value)
{
	if (Value != 0.0f) //if movement isnt zero
	{
		//add movement in the right direction
		AddMovementInput(GetActorRightVector(), Value);
		bIsIdle = false;
	}
	else 
	{
		if (!bIsGrounded) //if no right input and not grounded
			bIsIdle = false; //player not idle
		else
			bIsIdle = true;
	}
}

void ABirdPlayer::StartJump() 
{
	if (CanJump())
	{
		bIsGliding = false;
		Jump();
	}
}

void ABirdPlayer::StopJump()
{
	if (bIsGliding) 
	{
		//if gliding, ensure player isn't jumping
		StopJumping();
	}
}

void ABirdPlayer::StartSprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f; //sets sprint speed
}

void ABirdPlayer::StopSprint()
{
	bIsSprinting = false; 
	GetCharacterMovement()->MaxWalkSpeed = 300.0f; //sets walking speed
}

void ABirdPlayer::UpdateAnimations()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	if (bIsSprinting)
	{
		AnimInstance->Montage_Play(WalkingAnimation);
	}
	else if (bIsGliding)
	{
		AnimInstance->Montage_Play(GlidingAnimation);
	}
	else if (bIsFalling)
	{
		AnimInstance->Montage_Play(FallingAnimation);
	}
	else if (GetVelocity().Size() > 0)
	{
		AnimInstance->Montage_Play(WalkingAnimation);
	}
	else
	{
		AnimInstance->Montage_Play(IdleAnimation);
	}


}

// Called to bind functionality to input
void ABirdPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABirdPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABirdPlayer::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABirdPlayer::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABirdPlayer::StopJump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABirdPlayer::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABirdPlayer::StopSprint);
}

