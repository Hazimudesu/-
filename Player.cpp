#include <imgui.h>
#include "Player.h"
#include "Camera.h"
#include "Input/Input.h"
#include"Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "CameraController.h"
Player::Player()
{
    model = new Model("Data/Model/Mr.Incredible/tank.mdl");

    scale.x = scale.y = scale.z = 0.1f;
    hitEffect = new Effect("Data/Effect/Hit1.efkefc");

    //angle.y = -1.5f;
}

Player::~Player()
{
    delete hitEffect;
    delete model;
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    projectileManager.Render(dc, shader);
}
void Player::DrawDebugGUI()
{

    if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_DefaultOpen))
    {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(900, 900), ImGuiCond_FirstUseEver);
        // デバッグプリミティブ表示
        //ImGui::Checkbox("VisibleDebugPrimitive", &visibleDebugPrimitive);
        // ステート

        // 位置
        ImGui::InputFloat3("Position", &position.x);
        // 回転
        DirectX::XMFLOAT3 a;
        a.x = DirectX::XMConvertToDegrees(angle.x);
        a.y = DirectX::XMConvertToDegrees(angle.y);
        a.z = DirectX::XMConvertToDegrees(angle.z);
        ImGui::InputFloat3("Angle", &a.x);
        angle.x = DirectX::XMConvertToRadians(a.x);
        angle.y = DirectX::XMConvertToRadians(a.y);
        angle.z = DirectX::XMConvertToRadians(a.z);
        // スケール
        ImGui::InputFloat3("Scale", &scale.x);
        // 移動速度
        ImGui::InputFloat("MoveSpeed", &moveSpeed);
        // 旋回速度
        float turnDeg = DirectX::XMConvertToDegrees(turnSpeed);
        ImGui::InputFloat("TurnSpeed", &turnDeg);
        turnSpeed = DirectX::XMConvertToRadians(turnDeg);
        // 半径
        ImGui::InputFloat("Radius", &radius);
        // 高さ
        ImGui::InputFloat("Height", &height);
        // 摩擦
        ImGui::InputFloat("Friction", &friction);
        // 加速度
        ImGui::InputFloat("Acceleration", &acceleration);
        ////体力
        ImGui::InputFloat("Health", &health);
        
        //ImGui::InputFloat("Shot", &shotCollTime);
        ImGui::InputFloat("CollTimer", &timer);

        ImGui::Checkbox("CanShot", &canShot);
        // 空中制御係数
       /* ImGui::InputFloat("AirControl", &airControl);*/
    }
}

void Player::Update(float elapsedTime)
{

     UpdateVelocity(elapsedTime);

     InputMove(elapsedTime);

     projectileManager.Update(elapsedTime);
     InputProjectile();

     CollisionPlayerVsEnemies();

     CollisionProjectilesVsEnemies();

     model->UpdateTransform(transform);


     UpdateTransform();
     InputJump();
     
}
void Player::InputMove(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ay1 = gamePad.GetAxisLY();
    float ax1 = gamePad.GetAxisLX();
    

    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    Move(moveVec.x, moveVec.z, moveSpeed);
    
    if (!ay1)
    {
    Turn(elapsedTime, ax1, 0, turnSpeed);
    }
}

void Player::InputProjectile()
{
    GamePad& gamepad = Input::Instance().GetGamePad();

    // 直進弾丸発射
    if (gamepad.GetButtonDown() & GamePad::BTN_X && shotCollTime == 0)
    {
        // 前方向
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);
        // 発射位置（プレイヤーの腰あたり）
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;
        // 発射
        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);
        shotCollTime++;

    }
    if (shotCollTime > 0)
    {
        timer++;
        canShot = false;
    }
    if (timer == 300)
    {
        timer = 0;
        shotCollTime = 0;
        canShot = true;
    }
    ////追尾弾
    //if (gamepad.GetButtonDown() & GamePad::BTN_Y)
    //{
    //    // 前方向
    //    DirectX::XMFLOAT3 dir;
    //    dir.x = sinf(angle.y);
    //    dir.y = 0.0f;
    //    dir.z = cosf(angle.y);

    //    // 発射位置（プレイヤーの腰あたり）
    //    DirectX::XMFLOAT3 pos;
    //    pos.x = position.x;
    //    pos.y = position.y + height * 0.5f;
    //    pos.z = position.z;

    //    // ターゲット（デフォルトではプレイヤーの前方）
    //    DirectX::XMFLOAT3 target;
    //    target.x = pos.x + dir.x * 1000.0f;
    //    target.y = pos.y + dir.y * 1000.0f;
    //    target.z = pos.z + dir.z * 1000.0f;

    //    // 一番近くの敵をターゲットにする
    //    float dist = FLT_MAX;
    //    EnemyManager& enemyManager = EnemyManager::Instance();
    //    int enemyCount = enemyManager.GetEnemyCount();
    //    for (int i = 0; i < enemyCount; ++i)
    //    {
    //        // 敵との距離判定
    //        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
    //        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
    //        DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
    //        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
    //        DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
    //        float d;
    //        DirectX::XMStoreFloat(&d, D);
    //        if (d < dist)
    //        {
    //            dist = d;
    //            target = enemy->GetPosition();
    //            target.y += enemy->GetHeight() * 0.5f;
    //        }
    //    }

    //     発射
    //    ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
    //    projectile->Launch(dir, pos, target);

    //}
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    debugRenderer->DrawCylinder(position, radius,height, DirectX::XMFLOAT4(0, 0, 0, 1));

    projectileManager.DrawDebugPrimitive();
}

void Player::InputJump()
{

 /*   GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount < jumpLimit)
        {
            jumpCount++;
             Jump(jumpSpeed);
        }
    }*/

}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrt((cameraRightX * cameraRightX) + (cameraRightZ * cameraRightZ));
    if (cameraRightLength > 0.0f)
    {
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrt((cameraFrontX * cameraFrontX) + (cameraFrontZ * cameraFrontZ));
    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    DirectX::XMFLOAT3 vec;
    vec.x = (ay * cameraFrontX);
    vec.z = (ay * cameraFrontZ);
    vec.y = 0.0f;

    float speed = rollSpeed * 0.1f;

    return vec;
}





void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            position,
            radius,
            height,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition
        ))
        {

            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            
            // 上から踏んづけた場合は小ジャンプする
            if (normal.y > 0.8f)
            {
                // 小ジャンプする
                Jump(jumpSpeed * 0.5f);
            }
            else
            {
                // 押し出し後の位置設定
                enemy->SetPosition(outPosition);
            }
        }
    }
}

void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // 全ての弾丸と全ての敵を総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            // 衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                // ダメージを与える
                if (enemy->ApplyDamage(1, 0.5f))
                {
                    // 吹き飛ばす
                    {
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        DirectX::XMFLOAT3 impulse;
                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;

                        enemy->AddImpulse(impulse);
                    }
                    
                    //// ヒットエフェクト再生
                    {
                        DirectX::XMFLOAT3 e = enemy->GetPosition();
                        e.y += enemy->GetHeight() * 0.5f;
                        hitEffect->Play(e);
                    }

                    // 弾丸破棄
                    projectile->Destroy();
                }
            }
        }
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}
