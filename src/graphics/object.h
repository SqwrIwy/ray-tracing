#pragma once

#include "graphics/material.h"

namespace VCL {

class Object
{
public:

  const Material *mat_;

public:

  Object(const Material *const mat) : mat_(mat) { };

  virtual ~Object() = default;

  const Material *Mat() const { return mat_; }

  virtual real Intersect(const Ray &ray) const = 0;

  virtual Vec3 ClosestNormal(const Vec3 &pos) const = 0;
};

class Plane : public Object
{
protected:

  const Vec3 pos_;
  const Vec3 n_;

public:

  Plane(const Material *const mat, const Vec3 &pos, const Vec3 &dir) :
    Object(mat),
    pos_(pos),
    n_(dir.normalized())
  { }

  virtual ~Plane() = default;

  virtual real Intersect(const Ray &ray) const override
  {
    // Part 1, Step 1: your code here
    real t = (n_.dot(pos_ - ray.ori_)) / (n_.dot(ray.dir_));
    if (t < 0)
      return std::numeric_limits<real>::infinity();
    else
      return t;
  }

  virtual Vec3 ClosestNormal(const Vec3 &position) const { return n_; }
};

class Sphere : public Object
{
protected:

  const Vec3 cen_;
  const real rad_;

public:

  Sphere(const Material *const mat, const Vec3 &cen, const real rad) :
    Object(mat),
    cen_(cen),
    rad_(rad)
  { }

  virtual ~Sphere() = default;

  virtual real Intersect(const Ray &ray) const override
  {
    // Part 1, Step 1: your code here
    real a = ray.dir_.dot(ray.dir_);
    real b = ray.dir_.dot(ray.ori_ - cen_) * real(2);
    real c = (ray.ori_ - cen_).dot(ray.ori_ - cen_) - rad_ * rad_;
    real sqrt_delta = sqrt(b * b - real(4) * a * c);
    real t1 = (- b - sqrt_delta) / a / real(2);
    real t2 = (- b + sqrt_delta) / a / real(2);
    if (t1 < 0)
    {
      if (t2 < 0)
        return std::numeric_limits<real>::infinity();
      else
        return t2;
    } else
      return t1;
  }

  virtual Vec3 ClosestNormal(const Vec3 &pos) const { return (pos - cen_).normalized(); }
};

}