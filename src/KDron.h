#ifndef KDRON_H
#define KDRON_H

#include <GL/glew.h>

#include "indexmodel.h"
#include "movablemodel.h"
#include "modelprogram.h"
#include "matma.h"

class KDron : public IndexModel, public MovableModel {
public:
    KDron(float init_velocity = 10, float init_angle = 0);
    void SetInitAngle(float angle) { angle_ = angle; }
    void SetVelocity(float velocity) { velocity_ = velocity; }

    /*rotation can be improved by using quaternions*/
    void Rotate(float pitch, float yaw, float roll);
    void SetRotation(float pitch, float yaw, float roll);

    void Initialize();
    void Draw(const ModelProgram& program) const;
    void Move(float delta_t);
    void SpeedUp();
    void SlowDown();
    void ToggleAnimated();
private:
    float angle_;
    float velocity_;
    bool animated_;

    float pitch_ = 0, yaw_ = 0, roll_ = 0; //current angle
};

#endif // KDRON_H
