#include "globillum.h"

#include "common/helperfunc.h"
#include "light.h"

#include <iostream>

namespace VCL::GlobIllum {

Vec3 AxisAngle(const Vec3 &w, const real cos2theta, const real phi)
{
	const real cos_theta = std::sqrt(cos2theta);
	const real sin_theta = std::sqrt(1 - cos2theta);
	const Vec3 u = (std::abs(w[0]) > real(.1) ? Vec3(0, 1, 0) : Vec3(1, 0, 0)).cross(w).normalized();
	const Vec3 v = w.cross(u);
	return (u * std::cos(phi) * sin_theta + v * std::sin(phi) * sin_theta + w * cos_theta).normalized();
}

Vec3 Sample(const Material *const mat, const Vec3 &n, const Vec3 &wi, Color &weight)
{
      weight = mat->k_s_.any() ? mat->k_s_ : Color(0, 0, 0);
      return n * 2 * n.dot(wi) - wi;
  const real R = mat->k_d_.mean() / (mat->k_d_.mean() + mat->k_s_.mean());
  const real r0 = rand01();
  if (r0 < R) { // sample diffuse ray
    weight = mat->k_d_.any() ? mat->k_d_ / R : Color(0, 0, 0);
    return AxisAngle(n, rand01(), rand01() * 2 * PI_);
  }
  else { // sample specular ray
    if (mat->alpha_ >= 0) {
      const Vec3 d = AxisAngle(n * 2 * n.dot(wi) - wi, std::pow(rand01(), real(2) / (mat->alpha_ + 2)), rand01() * 2 * PI_);
      weight = n.dot(d) <= 0 || !mat->k_s_.any() ? Color(0, 0, 0) : mat->k_s_ / (1 - R);
      return d;
    }
    else { // for ideal mirrors
      weight = mat->k_s_.any() ? mat->k_s_ / (1 - R) : Color(0, 0, 0);
      return n * 2 * n.dot(wi) - wi;
    }
  }
}

Vec3 Sample(const Material *const mat, const Vec3 &n, const Vec3 &wi)
{
  const real R = mat->k_d_.mean() / (mat->k_d_.mean() + mat->k_s_.mean());
  const real r0 = rand01();
  if (r0 < R) { // sample diffuse ray
    real y = rand01();
    //pdf = real(1) / 2 / PI_ * R;
    return AxisAngle(n, y, rand01() * 2 * PI_);
  }
  else { // sample specular ray
    if (mat->alpha_ >= 0) {
      real y = std::pow(rand01(), real(2) / (mat->alpha_ + 2));
      const Vec3 d = AxisAngle(n * 2 * n.dot(wi) - wi, y, rand01() * 2 * PI_);
      //pdf = (mat->alpha_ / 2 + 1) * std::pow(y, mat->alpha_ / 2) / 2 / PI_ * (1 - R);
      return d;
    }
    else { // for ideal mirrors
      return n * 2 * n.dot(wi) - wi;
    }
  }
}

real norm(Vec3 x)
{
  return x.dot(x);
}

Color RayTrace(const Scene &scene, Ray ray)
{
  Color color(0, 0, 0);
  Color weight(1, 1, 1);
  std::vector<Light> lights;

  for (int depth = 0; depth < 10; depth++) {
    lights.clear();
    Vec3 pos;
    const Object *obj = scene.Intersect(ray, pos);
    if (!obj) return color;
    auto mat = obj->Mat();
    const Vec3 n = obj->ClosestNormal(pos);
    const Vec3 v = -ray.dir_;

    // Lights
    for (const auto& tlight : scene.lights_) {
      Vec3 test_pos;
      const Ray test_ray(pos + 0.01 * (tlight->position - pos), (tlight->position - pos).normalized());
      const Object * test_obj = scene.Intersect(test_ray, test_pos);
      if (test_obj && test_obj->Mat()->emissive_) {
        lights.push_back(*tlight);
      }
    }

    // Phong shading
    Color result(0, 0, 0);
    // Part 1, Step 2: your code here
    for (const auto& light : lights)
    {
      const Vec3 l = (light.position - pos).normalized();
      const Vec3 h = (v+l).normalized();
      result += mat->k_s_ * light.intensity / norm(light.position - pos) * std::pow(std::max(real(0), n.dot(h)), mat->alpha_);
      result += mat->k_d_ * light.intensity / norm(light.position - pos) * std::max(real(0), n.dot(l));
    }
    result += mat->k_d_ * scene.ambient_light_;

    // accumulate color
    Color R = mat->k_s_ * 0.5;
    color += weight * (Color(1, 1, 1) - R) * result;
    weight *= R;

    // generate new ray
    // Part 1, Step 2: your code here
    Color t;
    const Vec3 l = Sample(mat, n, v, t);
    weight *= t;
    ray = Ray(pos + 0.01 * l, l);
  }

  return color;
}

Color fr(const Material *mat, Vec3 n, Vec3 v, Vec3 l)
{
  Color result(0, 0, 0);
  const Vec3 h = (v + l).normalized();
  result += std::pow(std::max(real(0), n.dot(h)), mat->alpha_) * mat->k_s_;
  result += std::max(real(0), n.dot(l)) * mat->k_d_;
  return result;
}

/*Color pdf(const Material *mat, Vec3 n, Vec3 l)
{
  Color result(0, 0, 0);
  const real R = mat->k_d_.mean() / (mat->k_d_.mean() + mat->k_s_.mean());
  result += std::pow(std::max(real(0), n.dot(h)), mat->alpha_) * mat->k_s_;
  result += () * R;
  return result;
}*/

Color PathTrace(const Scene &scene, Ray ray)
{
  Color weight(1, 1, 1);

  real P_RR = 0.9;

  //for (int depth = 0; depth < 10; depth++)
  while (1)
  {
    if (rand01() > P_RR)
      return Color(0, 0, 0);
    weight /= P_RR;
    Vec3 pos;
    const Object *obj = scene.Intersect(ray, pos);
    if (!obj) {
      // Part 2, Step 2: your code here
      return Color(0, 0, 0);
    }
    if (obj->Mat()->emissive_) {
      // Part 2, Step 2: your code here
      for (const auto &tlight : scene.lights_)
      {
        /*Vec3 test_pos;
        const Ray test_ray(ray.ori_ + 0.01 * (tlight->position - ray.ori_), (tlight->position - ray.ori_).normalized());
        const Object *test_obj = scene.Intersect(test_ray, test_pos);
        if (test_obj && test_obj->Mat()->emissive_ && test_obj == obj)
        {*/
          return tlight->intensity * weight * 8;
        //}
      }
    }

    // Part 2, Step 2: your code here

    auto mat = obj->Mat();
    const Vec3 n = obj->ClosestNormal(pos);
    const Vec3 v = -ray.dir_;
    if (!weight.any())
      return weight;
    else {
      // Part 2, Step 2: your code here
      const Vec3 l = Sample(mat, n, v);
      weight *= fr(mat, n, v, l);
      ray = Ray(pos + 0.0001 * l, l);
    }
  }

  // Part 2, Step 2: your code here
  //return scene.ambient_light_ * weight;
  return Color(0, 0, 0);
}
}