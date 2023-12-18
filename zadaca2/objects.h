#pragma once
#include <cmath>
#include "geometry.h"
#include "ray.h"
#include "material.h"

class Object
{
  public:
    Material material;
    virtual bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const = 0;
};

class Sphere : public Object
{
    float r;
    Vec3f c;

  public:
    Sphere(const Vec3f &c, const float &r) : c(c), r(r) { }
    Sphere(const Vec3f &c, const float &r, const Material &mat) : c(c), r(r)
    {
        Object::material = mat;
    }

    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const
    {
        Vec3f e_minus_c = ray.origin - c;
        float d2 = ray.direction * ray.direction;

        float disc = pow(ray.direction * e_minus_c, 2) - d2 * (e_minus_c * e_minus_c - r * r);

        if (disc < 0)
        {
            return false;
        }
        else
        {
            bool ray_inside_sphere = e_minus_c * e_minus_c <= r * r;
            if (ray_inside_sphere)
            {
                float t1 = (-ray.direction * e_minus_c + sqrt(disc)) / d2;
                t = t1;
            }
            else
            {
                float t2 = (-ray.direction * e_minus_c - sqrt(disc)) / d2;
                t = t2;
            }

            Vec3f hit_point = ray.origin + ray.direction * t;
            normal = (hit_point - c).normalize();
            return true;
        }
    }
};
struct Cuboid : public Object{
    Vec3f a, b;

    public:
        Cuboid(const Vec3f &a, const Vec3f &b) : a(a), b(b) { }
        Cuboid(const Vec3f &a, const Vec3f &b, const Material &mat) : a(a), b(b){
            Object::material = mat;
        }

    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const{
        float tmin = 0;
        float tmax = 100000000;
        float minX = min(a[0], b[0]);
        float minY = min(a[1], b[1]);
        float minZ = min(a[2], b[2]);
        float maxX = max(a[0], b[0]);
        float maxY = max(a[1], b[1]);
        float maxZ = max(a[2], b[2]);

        if(ray.direction[0] == 0 && (ray.origin[0]<minX || ray.origin[0] > maxX)){
            return false;
        }

        else{
            float t1 = (minX - ray.origin[0]) / ray.direction[0];
            float t2 = (maxX - ray.origin[0]) / ray.direction[0];

            if(t1 > t2){
                swap(t1, t2);
            }

            tmin = max(tmin, t1);
            tmax = min(tmax, t2);

            if(tmin > tmax || tmax < 0){
                return false;
            }
        }

        t = tmin;

        if(ray.direction[1] == 0 && (ray.origin[1] < minY || ray.origin[1] > maxY)){
            return false;
        }

        else{
            float t1 = (minY - ray.origin[1]) / ray.direction[1];
            float t2 = (maxY - ray.origin[1]) / ray.direction[1];

            if(t1 > t2){
                swap(t1, t2);
            }

            tmin = max(tmin, t1);
            tmax = min(tmax, t2);

            if(tmin > tmax || tmax < 0){
                return false;
            }
        }

        t = tmin;

        if(ray.direction[2] == 0 && (ray.origin[2] < minZ || ray.origin[2] > maxZ)){
            return false;
        }

        else{
            float t1 = (minZ - ray.origin[2]) / ray.direction[2];
            float t2 = (maxZ - ray.origin[2]) / ray.direction[2];

            if(t1 > t2){
                swap(t1, t2);
            }

            tmin = max(tmin, t1);
            tmax = min(tmax, t2);

            if(tmin > tmax || tmax < 0){
                return false;
            }
        }

        t = tmin;

        if(abs(ray.origin[0] - a[0]) < 0.01){
            normal = Vec3f(-1,0,0);
        }

        else if(abs(ray.origin[0] - b[0]) < 0.01){
            normal = Vec3f(1, 0, 0);
        }

        else if(abs(ray.origin[1] - a[1]) < 0.01){
            normal = Vec3f(0, -1, 0);
        }

        else if(abs(ray.origin[1] - b[1]) < 0.01){
            normal = Vec3f(0, 1, 0);
        }

        else if(abs(ray.origin[2] - a[2]) < 0.01){
            normal = Vec3f(0, 0, -1);
        }

        else if(abs(ray.origin[2] - b[2]) < 0.01){
            normal = Vec3f(0, 0, 1);
        }

        return true;
    }
};