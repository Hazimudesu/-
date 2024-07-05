#include "EnemyManager.h"
#include "Collision.h"
//更新
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }
	
	for (Enemy* enemy : removes)
	{
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		delete enemy;
	}
	removes.clear();

    CollisionEnemyVsEnemies();
}

void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}

void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

void EnemyManager::Clear()
{
    enemies.clear();
}

void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugPrimitive();
    }
}

void EnemyManager::Remove(Enemy* enemy)
{
	removes.insert(enemy);
}
//エネミー同士の衝突判定
void EnemyManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemyA = enemies.at(i);
		for (int j = i + 1; j < enemyCount; ++j)
		{
			Enemy* enemyB = enemies.at(j);

			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsCylinder(
				enemyA->GetPosition(),
				enemyA->GetRadius(),
				enemyA->GetHeight(),
				enemyB->GetPosition(),
				enemyB->GetRadius(),
				enemyB->GetHeight(),
				outPosition))
			{
				enemyB->SetPosition(outPosition);
			}
		}
	}
}


// デバッグ用GUI描画
void EnemyManager::DrawDebugGUI()
{
	if (ImGui::CollapsingHeader("EnemyManager", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Add"))
		{
			DirectX::XMFLOAT3 position =
			{
				((static_cast<float>(rand() * 2) / RAND_MAX) - 1.0f) * 5.0f,
				0.0f,
				((static_cast<float>(rand() * 2) / RAND_MAX) - 1.0f) * 5.0f,
			};
			Enemy* enemy = new EnemySlime();
			enemy->SetPosition(position);
			Register(enemy);
		}
		ImGui::SameLine();

		if (ImGui::Button("Clear"))
		{
			Clear();
		}

		//ImGui::Checkbox("VisibleDebugPrimitive", &visibleDebugPrimitive);

		int index = 0;
		for (Enemy* enemy : enemies)
		{
			char name[32];
			::sprintf_s(name, sizeof(name), "%d", index);

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::TreeNodeEx(enemy, node_flags, name))
			{
				ImGui::PushID(index);
				enemy->DrawDebugGUI();
				ImGui::PopID();
				ImGui::TreePop();
			}
			index++;
		}
	}
}
