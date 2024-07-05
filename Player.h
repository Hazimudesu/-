#pragma once
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include "Input/Mouse.h"


class Player : public Character
{
public:
    Player();
    ~Player() override;

    void Update(float elapsedTime);

    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //デバッグ用GUI描画
    void DrawDebugGUI();

    void DrawDebugPrimitive();

   
    void InputJump();

private:
    Model* model = nullptr;

    DirectX::XMFLOAT3 GetMoveVec() const;


    void InputMove(float elapsedTime);

    void InputProjectile();

    void CollisionPlayerVsEnemies();

    void CollisionProjectilesVsEnemies();

    ProjectileManager projectileManager;


protected:
    void OnLanding() override;
private:

    float   moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(90);

    float jumpSpeed = 20.0f;
    
    int jumpCount = 0;
    int jumpLimit = 2;
    Effect* hitEffect = nullptr;

    float               rollSpeed = DirectX::XMConvertToRadians(90);

    float shotCollTime = 0;

    float timer = 0;

    bool canShot = true;
};
