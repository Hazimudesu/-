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
    
    // ��������
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        // �������폜
        Destroy();
    }

    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;
    position.y += direction.y * speed;
    position.z += direction.z * speed;


    UpdateTransform();

    model->UpdateTransform(transform);


    // �����ړ��l
    float mx = position.x * elapsedTime;
    float mz = position.z * elapsedTime;

    // ���C�̊J�n�ʒu�ƏI�_�ʒu
    DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
    DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

    // ���C�L���X�g�ɂ��ǔ���
    HitResult hit;
    if (StageManager::Instance().RayCast(start, end, hit))
    {
        // �ǂ܂ł̃x�N�g��
        DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
        DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

        // �ǂ̖@��
        DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

        // ���˃x�N�g����@���Ɏˉe
        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

        // �␳�ʒu�̌v�Z
        DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
        DirectX::XMFLOAT3 collectPosition;
        DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

        // �ǂ�������փ��C�L���X�g
        HitResult hit2;
        if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
        {
            // �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
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
