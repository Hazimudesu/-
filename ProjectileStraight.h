#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"

class ProjectileStraight : public Projectile
{
public:
    ProjectileStraight(ProjectileManager* manager);
    ~ProjectileStraight() override;

    //�X�V����
    void Update(float elapsedTime) override;

    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTimer = 3.0f;
    DirectX::XMFLOAT3 velocity = { 0,0,0 };
    float stepOffset = 1.0f;
};