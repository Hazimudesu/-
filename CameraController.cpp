#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    float speed = rollSpeed * elapsedTime;
    if(!ay)
    {
        angle.y += ax * speed;

    }
    //if (angle.x < minAngleX)
    //{
    //    angle.x = minAngleX;
    //}
    //if (angle.x > maxAngleX)
    //{
    //   angle.x = maxAngleX;
    //}
    //if (angle.y < -DirectX::XM_PI)
    //{
    //    angle.y += DirectX::XM_2PI;
    //}
    //if (angle.y > DirectX::XM_PI)
    //{
    //    angle.y -= DirectX::XM_2PI;
    //}
     




    //ÉJÉÅÉââÒì]ílÇâÒì]çsóÒÇ…ïœä∑    
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    DirectX::XMFLOAT3 eye;

    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

        Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
