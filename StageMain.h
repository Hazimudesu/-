#pragma once

#include "Graphics/Model.h"
#include "Stage.h"
class StageMain : public Stage
{
public:
    StageMain();
    ~StageMain() override;

    //static Stage& Instance();

    void Update(float elapsedTime) override;

    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;
private:
    Model* model = nullptr;
};
