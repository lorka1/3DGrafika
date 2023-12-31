#pragma once
#include "geometry.h"

class Material
{
  public:
    Vec3f diffuse_colour;
    float diffuse_coef = 1;
    
    float specular_coef = 1;
    float phong_exp = 1000;
    float reflectivity_coef = 0.1;
    float opacity = 0.1;
    
    Material() : diffuse_colour(Vec3f(0, 0, 0)) { }
    Material(const Vec3f &colour) : diffuse_colour(colour) { }
};