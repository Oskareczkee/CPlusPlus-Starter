#ifndef planet_uranus_H
#define planet_uranus_H

#include "indexmodel.h"
#include "movablemodel.h"
#include "texturemodel.h"
#include "matma.h"
#include "texturecameraprogram.h"

class Sphere : public IndexModel, public MovableModel, public TextureModel {
public:
    Sphere(float init_velocity = 40, float init_angle = 0);
    void SetInitAngle(float angle) { angle_ = angle; }
    void SetVelocity(float velocity) { velocity_ = velocity; }
    void Initialize(int n, int m, float R);
    void Draw(const TextureCameraProgram& program) const;
    void MakeReadyForTransforms();
    void Move(float delta_t);
    void Translate(float xOffset, float yOffset, float zOffset);
    void SpeedUp();
    void SlowDown();
    void ToggleAnimated();
private:
    int n_; // mesh parameter
    int m_; // mesh parameter

    float R; // radius

    float angle_;
    float velocity_;
    bool animated_;


    float xOffset = 0, yOffset = 0, zOffset = 0;
};

#endif // planet_uranus_H
