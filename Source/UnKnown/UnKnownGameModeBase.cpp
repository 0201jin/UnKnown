// Fill out your copyright notice in the Description page of Project Settings.

#include "UnKnownGameModeBase.h"

#include "Player/Hero.h"

AUnKnownGameModeBase::AUnKnownGameModeBase()
{
	static ConstructorHelpers::FObjectFinder<UClass> HUD(TEXT("Blueprint'/Game/UI/BP/MyHUD.MyHUD_C'"));
	UClass * _hud = HUD.Object;
	//PlayerControllerClass = AMyPlayerController::StaticClass();
	DefaultPawnClass = AHero::StaticClass();
	//GameStateClass = ACustomGameState::StaticClass();
	HUDClass = _hud;
	//ReplaySpectatorPlayerControllerClass = ACustomReplaySpectatorPlayerController::StaticClass();
	//SpectatorClass = ACustomSpectatorClass::StaticClass();
}

AUnKnownGameModeBase::~AUnKnownGameModeBase()
{
}
