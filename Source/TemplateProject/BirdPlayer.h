// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BirdPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;


UCLASS()
class TEMPLATEPROJECT_API ABirdPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABirdPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//player movement functions
	void MoveForward(float value);
	void MoveRight(float value);

	//player jump functions
	void StartJump();
	void StopJump();

	//player sprint functions
	void StartSprint();
	void StopSprint();

	//player animations
	void UpdateAnimations();

	//input bindings
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private: 
	//declare SpringArm and Camera components
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	//movement states
	bool bIsSprinting;
	bool bIsGrounded;
	bool bIsJumping;
	bool bIsGliding;
	bool bIsFalling;
	bool bIsIdle;

public: 
	//animation variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* IdleAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* WalkingAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* JumpingAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* LandingAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* GlidingAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* FallingAnimation;


};
