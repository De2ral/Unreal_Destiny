// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLegacyCameraShake.h"

UMyLegacyCameraShake::UMyLegacyCameraShake()
{
    // Oscillation settings for the camera shake
    OscillationDuration = 0.25f;
    OscillationBlendInTime = 0.05f;
    OscillationBlendOutTime = 0.05f;

    RotOscillation.Pitch.Amplitude = 1.0f;
    RotOscillation.Pitch.Frequency = FMath::RandRange(25.0f, 35.0f);

    RotOscillation.Yaw.Amplitude = 1.0f;
    RotOscillation.Yaw.Frequency = FMath::RandRange(25.0f, 35.0f);



    //RotOscillation.Pitch.Amplitude = FMath::RandRange(2.0f, 4.0f);
    //RotOscillation.Pitch.Frequency = FMath::RandRange(25.0f, 35.0f);
//
    //RotOscillation.Yaw.Amplitude = FMath::RandRange(1.0f, 2.0f);
    //RotOscillation.Yaw.Frequency = FMath::RandRange(25.0f, 35.0f);
}