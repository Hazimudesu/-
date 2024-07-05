#pragma once

#include<vector>
#include "Enemy.h"
#include <set>
#include "imgui.h"
#include "EnemySlime.h"
class EnemyManager
{
private:
    EnemyManager() {}   
    ~EnemyManager() {}

    void CollisionEnemyVsEnemies();

public:
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Update(float elapsedTime);

    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void Register(Enemy* enemy);

    void Clear();

    void DrawDebugPrimitive();

    void DrawDebugGUI();
    void Remove(Enemy* enemy);

    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    Enemy* GetEnemy(int index) { return enemies.at(index); }

private:
    std::vector<Enemy*> enemies;

    std::set<Enemy*> removes;
};

