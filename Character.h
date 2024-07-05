#pragma once

#include <DirectXMath.h>
#include "imgui.h"
#include "Input/GamePad.h"
#include "Input/Input.h"
class Character
{
public:
    Character() {}
    virtual ~Character() {}

    void UpdateTransform();

    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    float GetHeight() const { return height; }
    float GetRadius() const { return radius; }
    bool IsGround() const { return isGround; }

    bool ApplyDamage(int damage, float invicibleTime);

    int GetHealth() const { return health; }

    int GetMaxHealth() const { return maxHealth; }
protected:
    void Move(float vx, float vz, float speed);
    void Turn(float elapsedTime, float vx, float vz, float speed);

    void Jump(float speed);

    void UpdateVelocity(float elapsedTime);

    void UpdateInvincibleTimer(float elapsedTime);

    virtual void OnLanding() {}
    
    virtual void OnDamaged() {}

    virtual void OnDead() {}

private:
    void UpdateVerticalVelocity(float elapsedFrame);

    void UpdateVerticalMove(float elapsedTime);

    void UpdateHorizontalVelocity(float elapsedFrame);

    void UpdateHorizontalMove(float elapsedTime);

protected:
    DirectX::XMFLOAT3   position = { 0, 0, 0 };
    DirectX::XMFLOAT3   angle = { 0, 0, 0 };
    DirectX::XMFLOAT3   scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4 transform = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    float radius = 0.5f;

    float gravity = -1.0f;
    DirectX::XMFLOAT3 velocity = { 0,0,0 };
    

    float health = 5;
    int maxHealth = 5;

    bool isGround = false;

    float height = 2.0f;

    float invincibleTimer = 1.0f;

    float friction = 0.5f;

    
    float acceleration = 1.0f;
    float maxMoveSpeed = 5.0f;
    float moveVecX = 0.0f;
    float moveVecZ = 0.0f;

    float stepOffset = 1.0f;

    float slopeRate = 1.0f;

    float               rollSpeed = DirectX::XMConvertToRadians(90);


};
