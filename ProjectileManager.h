#pragma once

#include <set>
#include <vector>
#include "Projectile.h"

class ProjectileManager
{
public:
    ProjectileManager();
    ~ProjectileManager();

    void Update(float elapsedTime);

    void Render(ID3D11DeviceContext* context, Shader* shader);

    void DrawDebugPrimitive();

    void Register(Projectile* projectile);

    void Clear();
    int GetProjectileCount() const { return static_cast<int>(projectiles.size()); }

    Projectile* GetProjectile(int index) { return projectiles.at(index); }

    void Remove(Projectile* projectile);
private:
    std::vector<Projectile*> projectiles;

    std::set<Projectile*> removes;
};