#include <iostream>
#include <limits>
#include <cmath>
#include <fstream>
#include <vector>
#include "geometry.h"

struct Sphere {
    Vec3f center;
    float radius;

    Sphere(const Vec3f &c, const float &r) : center(c), radius(r) {}

    bool rayIntersect(const Vec3f &origin, const Vec3f &dir, float &t0) const {
        Vec3f L = center - origin;
        float tca = L * dir;
        float d2 = L*L - tca*tca;

        if (d2 > radius*radius) return false;

        float thc = sqrtf(radius*radius - d2);
        t0       = tca - thc;
        float t1 = tca + thc;

        if (t0 < 0) t0 = t1;

        return t0 >= 0;
    }
};

Vec3f castRay(const Vec3f &origin, const Vec3f &dir, const Sphere &sphere) {
    float sphere_dist = std::numeric_limits<float>::max();

    if (!sphere.rayIntersect(origin, dir, sphere_dist)) {
        return {0.2, 0.7, 0.8};
    }

    return {0.4, 0.4, 0.3};
}

void render(const Sphere &sphere)
{
    const int width = 1024;
    const int height = 786;
    const float fov = M_PI/3.;
    std::vector<Vec3f> framebuffer(width * height);

    for (size_t j = 0; j < height; ++j) {
        for (size_t i = 0; i < width; ++i) {
//            framebuffer[i + j * width] = Vec3f(j/float(height), i/float(width), 0);
            float x =  (2*(i + 0.5)/(float)width  - 1)*tan(fov/2.)*width/(float)height;
            float y = -(2*(j + 0.5)/(float)height - 1)*tan(fov/2.);
            Vec3f dir = Vec3f(x, y, -1).normalize();
            framebuffer[i+j*width] = castRay(Vec3f(0,0,0), dir, sphere);
        }
    }

    std::ofstream ofs;

    ofs.open("./output.ppm", std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for (size_t i = 0; i < height * width; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }

    ofs.close();
}

int main() {
    std::cout << "Render the output" << std::endl;

    Sphere sphere = Sphere(Vec3f(-1.0, -1.5, -12), 2);
    render(sphere);

    return 0;
}
