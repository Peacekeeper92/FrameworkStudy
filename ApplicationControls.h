#pragma once

void CameraForward(Camera* const Cam);
void CameraBackward(Camera* const Cam);
void CameraLeft(Camera* const Cam);
void CameraRight(Camera* const Cam);
void CameraUp(Camera* const Cam);
void CameraDown(Camera* const Cam);

void CameraRotatePitch(Camera* const Cam, int dx, int dy);
void CameraRotateYaw(Camera* const Cam, int dx, int dy);