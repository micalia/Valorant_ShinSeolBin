// Fill out your copyright notice in the Description page of Project Settings.


#include "SB_FireComponent.h"
#include "Sound/SoundCue.h"

USB_FireComponent::USB_FireComponent() {
	static ConstructorHelpers::FObjectFinder<USoundCue> tempFireSound(TEXT("/Script/Engine.SoundCue'/Game/SB/Sounds/Gun/SC_SovaGun.SC_SovaGun'"));
	if (tempFireSound.Succeeded()) {
		FireSound = tempFireSound.Object;
	}
}

