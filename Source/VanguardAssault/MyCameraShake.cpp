#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
    OscillationDuration = 1.0f;
    OscillationBlendInTime = 0.05f;
    OscillationBlendOutTime = 0.2f;


    RotOscillation.Pitch.Amplitude = 0.0f;
    RotOscillation.Yaw.Amplitude = 0.0f;
    RotOscillation.Roll.Amplitude = 0.0f;

    LocOscillation.X.Amplitude = 0.0f;
    LocOscillation.X.Frequency = 0.0f;

    LocOscillation.Y.Amplitude = 20.0f;
    LocOscillation.Y.Frequency = 80.0f;

    LocOscillation.Z.Amplitude = 15.0f;
    LocOscillation.Z.Frequency = 60.0f;
}
