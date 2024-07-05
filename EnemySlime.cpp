#include "EnemySlime.h"

EnemySlime::EnemySlime()
{
    model = new Model("Data/Model/Slime/tank.mdl");

    scale.x = scale.y = scale.z = 0.05f;

    radius = 0.5f;
    height = 1.0f;
}

EnemySlime::~EnemySlime()
{
    delete model;
}

void EnemySlime::Update(float elapsedTime)
{
    UpdateVelocity(elapsedTime);

    UpdateTransform();

    UpdateInvincibleTimer(elapsedTime);

    model->UpdateTransform(transform);
}

void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void EnemySlime::OnDead()
{
    Destroy();
}


