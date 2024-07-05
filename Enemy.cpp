#include "Enemy.h"
#include "Graphics/Graphics.h"
#include"EnemyManager.h"
void Enemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    debugRenderer->DrawCylinder(position, radius,height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}

void Enemy::DrawDebugGUI()
{
	// �ʒu
	ImGui::InputFloat3("Position", &position.x);
	// ���a
	ImGui::InputFloat("Radius", &radius);
	// ����
	ImGui::InputFloat("Height", &height);
	// ���N
	//ImGui::InputInt("Health", &health);
}