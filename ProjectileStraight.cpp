#include "ProjectileStraight.h"
#include "Collision.h"
#include "StageManager.h"
ProjectileStraight::ProjectileStraight(ProjectileManager* manager) : Projectile(manager)
{
    model = new Model("Data/Model/Sword/Sword.mdl");

    scale.x = scale.y = scale.z = 3.0f;
}

ProjectileStraight::~ProjectileStraight()
{
    delete model;
}

void ProjectileStraight::Update(float elapsedTime)
{
    
    // 寿命処理
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        // 自分を削除
        Destroy();
    }

    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;
    position.y += direction.y * speed;
    position.z += direction.z * speed;


    UpdateTransform();

    model->UpdateTransform(transform);


    // 水平移動値
    float mx = position.x * elapsedTime;
    float mz = position.z * elapsedTime;

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
            Destroy();
        }

        
    }
}

void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}
