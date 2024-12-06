// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdGameMode.h"
#include "BirdPlayer.h"

ABirdGameMode::ABirdGameMode()
{
	//set default pawn class to my bird player character
	DefaultPawnClass = ABirdPlayer::StaticClass();
}

