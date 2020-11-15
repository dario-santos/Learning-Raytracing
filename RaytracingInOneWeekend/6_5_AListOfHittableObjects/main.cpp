#include "Color.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"
#include "Sphere.hpp"
#include "Hittable.hpp"

#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

color ray_color(const ray& r, const vector<hittable*> &objects)
{
  hit_record rec = hit_record();
 
  // If it's a hit, draw the sphere color
  for(auto o : objects)
    if (o->hit(r, -1.0f, 1000000.0f, rec))
      return 0.5 * color(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);

  // If there was no hit
  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);

  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
  ofstream myfile;
  myfile.open("output.ppm");

  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  // Camera
  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;
  auto focal_length = 1.0;

  auto origin = point3(0, 0, 0);
  auto horizontal = vec3(viewport_width, 0, 0);
  auto vertical = vec3(0, viewport_height, 0);
  auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

  // Scene
  vector<hittable*> objects;
  objects.push_back(new sphere(point3(0, 0, -1), 0.50));
  objects.push_back(new sphere(point3(0, 0.5, -1), 0.25));

  // Render
  myfile << "P3\n" << image_width << " " << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) 
  {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) 
    {
      auto u = double(i) / (image_width - 1);
      auto v = double(j) / (image_height - 1);
      ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
      color pixel_color = ray_color(r, objects);
      write_color(myfile, pixel_color);
    }
  }

  std::cerr << "\nDone.\n";
  myfile.close();
}
