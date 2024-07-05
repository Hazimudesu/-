#pragma once

class Scene
{
public:
    Scene() {}
    virtual ~Scene() {}

    bool IsReady() const { return ready; }

    void SetReady() { ready = true; }

    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
    virtual void Update(float elapsedTime) = 0;
    virtual void Render() = 0;
private:
    bool ready = false;
};