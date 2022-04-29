#include "renderer.h"

#include "common/helperfunc.h"
#include "graphics/globillum.h"

namespace VCL {
void Renderer::Init(const std::string& title, int width, int height) {
  width_ = width;
  height_ = height;
  InitPlatform();
  window_ = CreateVWindow(title, width_, height_, this);
  framebuffer_ = new Framebuffer(width_, height_);

  camera_ = new Camera;
  const float c_y = 1.5;
  const float c_z = 1.5 + 1.5 * std::sqrt(2);
  camera_->InitData((float)width_ / height_, 0.25f * PI_, 1.0f, 1000.0f, c_z,
                    0.0f, 0.5f * PI_, Vec3f(0, c_y, 0));

  auto &mats = scene_.mats_;
	auto &objs = scene_.objs_;
  auto &lights = scene_.lights_;
	// Initialize materials.
	mats["ceiling"] = std::make_unique<Material>(Color(1, 0, 0));
	mats["floor"] = std::make_unique<Material>(Color(0, 1, 0));
	mats["wall"] = std::make_unique<Material>(Color(0, 0, 1));
	mats["side"] = std::make_unique<Material>(Color(0, 1, 1));
	mats["wood"] = std::make_unique<Material>(Color(1, 1, 0));
	mats["metal"] = std::make_unique<Material>(Color(0, 0, 0), Color(.8f, .8f, .8f), 30);
	mats["mirror"] = std::make_unique<Material>(Color(37.2f, 24.4f, 13.2f) / 255, Color(.6f, .6f, .6f), -1);
	mats["light"] = std::make_unique<Material>(Color(20, 20, 20), true);
	// Set boundaries.
	objs.emplace_back(std::make_unique<Plane>(
		mats["ceiling"].get(),
		Vec3(0, 3, 0), Vec3(0, -1, 0)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["floor"].get(),
		Vec3(0, 0, 0), Vec3(0, 1, 0)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["side"].get(),
		Vec3(-2, 0, 0), Vec3(1, 0, 0)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["side"].get(),
		Vec3(2, 0, 0), Vec3(-1, 0, 0)));
	objs.emplace_back(std::make_unique<Plane>(
		mats["wall"].get(),
		Vec3(0, 0, -4), Vec3(0, 0, 1)));
	// Set the light.
	const real dl = real(2) / 3;
	const real rl = 5;
	const real hl = std::sqrt(rl * rl - dl * dl);
	objs.emplace_back(std::make_unique<Sphere>(
		mats["light"].get(),
		Vec3(0, 3 + hl, -2), rl));
  // Set internal objects.
  objs.emplace_back(std::make_unique<Sphere>(
			mats["wood"].get(),
			Vec3(1, real(.8), -3), real(.8)));
  objs.emplace_back(std::make_unique<Sphere>(
    mats["metal"].get(),
    Vec3(-1, real(.8), -2), real(.8)));
  lights.emplace_back(std::make_unique<Light>(
    Vec3(0, 3, -2),
    Color(1, 1, 1) * 2.0
  ));
  scene_.ambient_light_ = Color(0.05, 0.05, 0.05);
}

void Renderer::Progress(int &x, int &y, Color **buffer, int **cnt, const bool MonteCarlo) {
  const real dx = real(1) / width_;
	const real dy = real(1) / height_;

  const real lx = dx * x;
  const real ly = dy * y;

  const real sx = lx + rand01() * dx;
  const real sy = ly + rand01() * dy;

  if (!MonteCarlo) {
    buffer[y][x] += (GlobIllum::RayTrace(scene_, camera_->GenerateRay(sx, sy)) - buffer[y][x]) / (++cnt[y][x]);    
  }
  else {
    buffer[y][x] += (GlobIllum::PathTrace(scene_, camera_->GenerateRay(sx, sy)) - buffer[y][x]) / (++cnt[y][x]);
  }

  int idx = (y * width_ + x) * 4;
  for (int i = 0; i < 3; i++) framebuffer_->color_[idx + i] = std::round(std::pow(std::clamp(buffer[y][x][i], 0.0f, 1.0f), 1 / 2.2f) * 255);

  x++;
  if (x == width_) {
    x = 0;
    y++;
    if (y == height_) y = 0;
  }
}

void Renderer::MainLoop() {
  // switch between ray tracing and path tracing
  const bool MonteCarlo = false;

  int x;
  int y;

  Color **buffer = new Color *[height_];
  int **cnt = new int *[height_];

  for (y = 0; y < height_; y++) {
    buffer[y] = new Color[width_];
    cnt[y] = new int[width_];
    for (x = 0; x < width_; x++) {
      buffer[y][x] = Color::Zero();
      cnt[y][x] = 0;
    }
  }

  x = y = 0;
  int idx = 0;
  const int buffer_size = height_ * width_;
  int patch_size = 50000;
  patch_size = patch_size > buffer_size ? buffer_size : patch_size;
  while (!window_->should_close_) {
    PollInputEvents();

    // serial version
    // for (int i = 0; i < 10; ++i) {
    //   Progress(x, y, buffer, cnt, MonteCarlo);
    // }

    // parallel version
    # pragma omp parallel for
    for (int i = 0; i < patch_size; ++i) {
      int p = (idx + i) % buffer_size;
      int px = p % width_;
      int py = p / width_;
      Progress(px, py, buffer, cnt, MonteCarlo);
    }
    idx = (idx + patch_size) % buffer_size;

    window_->DrawBuffer(framebuffer_);
  }

  for (y = 0; y < height_; y++) {
    delete[] buffer[y];
    delete[] cnt[y];
  }
  delete[] buffer;
  delete[] cnt;
}

void Renderer::Destroy() {
  if (camera_) delete camera_;
  if (framebuffer_) delete framebuffer_;
  window_->Destroy();
  if (window_) delete window_;
  DestroyPlatform();
}
};  // namespace VCL