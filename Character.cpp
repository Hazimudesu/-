#include "Character.h"
#include "StageManager.h"
#include "Mathf.h"
void Character::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

    DirectX::XMMATRIX R = X * Y * Z;
    
    
    DirectX::XMMATRIX W = S * R * T;

    DirectX::XMStoreFloat4x4(&transform, W);
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

bool Character::ApplyDamage(int damage, float invincibleTime)
{
    if (damage == 0) return false;

    if (health <= 0) return false;

    if (invincibleTimer > 0.0f) return false;

    // 無敵時間設定
    invincibleTimer = invincibleTime;

    health --;

    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        OnDamaged();
    }
    return true;
}

void Character::Move(float vx, float vz, float speed)
{
    moveVecX = vx;
    moveVecZ = vz;

    maxMoveSpeed = speed;
}
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ay = gamePad.GetAxisLY();
    float ax = gamePad.GetAxisLX();
    speed *= elapsedTime;
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.0001f)
    {
        return;
    }
  //  vx /= length;
  //  vz /= length;

  //  float frontX = sinf(angle.y);
  //  float frontZ = cosf(angle.y);
  //  float dot = (vx * frontX) + (vz * frontZ);

  //  float rot = 1.0f - dot;
  ///*  if (rot > speed) rot = speed;*/
  //  float cross = (vz * frontX) - (vx * frontZ);
    
    if (ax > 0.0f)
    {
        angle.y += speed;
         //angle.y += speed;
    }
    else
    {
        angle.y -= speed;
         //angle.y -= speed;
    }

    //if (angle.y > 80)
    //{
    //    angle.y += rollSpeed;
    //}
    // if (angle.y < -80)
    //{
    //    angle.y -= rollSpeed;
    //}

}


void Character::UpdateVelocity(float elapsedTime)
{
    float elapsedFrame = 60.0f * elapsedTime;

    UpdateVerticalVelocity(elapsedFrame);

    UpdateHorizontalVelocity(elapsedFrame);

    UpdateVerticalMove(elapsedTime);

    UpdateHorizontalMove(elapsedTime);


}
void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    velocity.y += gravity * elapsedFrame;
}
void Character::UpdateVerticalMove(float elapsedTime)
{
    float my = velocity.y * elapsedTime;

    // キャラクターのY軸方向となる法線ベクトル
    DirectX::XMFLOAT3 normal = { 0, 1, 0 };

    slopeRate = 0.0f;

    // 落下中
    if (my < 0.0f)
    {
        // レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
        // レイの終点位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

        // レイキャストによる地面判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            // 地面に接地している
            position = hit.position;

            //回転
            angle.y += hit.rotation.y;

            // 傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

            // 法線ベクトル取得
            normal = hit.normal;
            // 着地した
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            // 空中に浮いている
            position.y += my;
            isGround = false;
        }
    }
    // 上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }


    {
        float ax = atan2f(normal.z, normal.y);
        float az = -atan2f(normal.x, normal.y);
        angle.x = Mathf::Lerp(angle.x, ax, 0.2f);
        angle.z = Mathf::Lerp(angle.z, az, 0.2f);
    }
}
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (length > 0.0f)
    {
        // 摩擦力
        float friction = this->friction * elapsedFrame;


        if (!isGround) friction *= 0.3f;
        // 摩擦による横方向の減速処理
        if (length > friction)
        {
            // 単位ベクトル化
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        // 横方向の速力が摩擦力以下になったので速力を無効化
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }
    // XZ平面の速力を加速する
    if (length <= maxMoveSpeed)
    {
        // 移動ベクトルがゼロベクトルでないなら加速する
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            // 加速力
            float acceleration = this->acceleration * elapsedFrame;

            // 空中にいるときは加速力を減らす
            if (!isGround) acceleration *= 0.3f;

            // 移動ベクトルによる加速処理
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            // 最大速度制限
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }
            // 下り坂でガタガタしないようにする
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }
            
        }
    }
    // 移動ベクトルをリセット
    moveVecX = 0.0f;
    moveVecZ = 0.0f;

}
void Character::UpdateHorizontalMove(float elapsedTime)
{
    // 水平速力量計算
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        // 水平移動値
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        // レイの開始位置と終点位置
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

        // レイキャストによる壁判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            // 壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

            // 壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            // 入射ベクトルを法線に射影
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

            // 補正位置の計算
            DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 collectPosition;
            DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

            // 壁ずり方向へレイキャスト
            HitResult hit2;
            if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
            {
                // 壁ずり方向で壁に当たらなかったら補正位置に移動
                position.x = collectPosition.x;
                position.z = collectPosition.z;
            }
            else
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;
            }
        }
        else
        {
            // 移動
            position.x += mx;
            position.z += mz;
        }
    }
    
}
void Character::Jump(float speed)
{
    velocity.y = speed;
}

