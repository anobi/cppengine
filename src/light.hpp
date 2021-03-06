#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "entitycomponent.hpp"

class Light : public EntityComponent {
public:

    Light() {};
    Light(const glm::fvec3 color, float intensity, float cutoff) : EntityComponent()
    {
        this->name = "Light";
        this->mColor = color;
        this->mIntensity = intensity;
        this->mCutoff = cutoff;
    };
    virtual ~Light() { }

    void SetColor(const glm::fvec3 color) { this->mColor = color; }
    void SetIntensity(const float i) { this->mIntensity = i; }
    void SetCutoff(const float cutoff) { this->mCutoff = cutoff; }

    const glm::fvec3 GetColor() { return this->mColor; }
    const float& GetIntensity() { return this->mIntensity; }
    const float& GetCutoff() { return this->mCutoff; }

protected:
    float mIntensity;
    float mCutoff;
    glm::fvec3 mColor;
};

class DirectionalLight : public Light
{
public:
    DirectionalLight() {};
    DirectionalLight(const glm::fvec3 color, float intensity, float maxDistance) : Light(color, intensity, maxDistance)
    {
        this->name = "DirectionalLight";
    };

    ~DirectionalLight() {};

    glm::fvec3 direction;
};

class PointLight : public Light {
public:
    PointLight() {};
    PointLight(const glm::fvec3 color, float intensity, float cutoff, float radius) : Light(color, intensity, cutoff)
    {
        this->name = "PointLight";
        this->SetRadius(radius);
    };
    ~PointLight() {};

    void SetRadius(const float r) { this->mRadius = r; }
    const float& GetRadius() { return this->mRadius; }

protected:
    float mRadius;
};

class SpotLight : public Light {
public:
    SpotLight() {};
    SpotLight(const glm::fvec3 color, float intensity, float maxDistance) : Light(color, intensity, maxDistance)
    {
        this->name = "SpotLight";
    };
    ~SpotLight() {};

    float angle;
    glm::fvec3 direction;
};

#endif
