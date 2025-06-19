#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1)); // 若光线击中物体，则物体的颜色为击中点normal的颜色映射
    }

    vec3 unit_direction = unit_vector(r.direction()); // unit_vector的y方向范围为[-1,1]
    auto a = 0.5 * (unit_direction.y() + 1.0); // 而我们希望alpha在[0,1]的范围内波动
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // 把随光线的y方向变化的alpha，带入blending的公式，得到最终的颜色
    // 最终实现随光线的y方向变化的蓝白渐变，当y为-1时alpha为0，所以最下面的部分为白色；当y为1时alpha为1，所以最上面的部分为蓝色
}

int main() {

	// Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio); // 计算结果会被截断为int
    image_height = (image_height < 1) ? 1 : image_height; // 防止height为0或者负数

    // World

    hittable_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera

    auto focal_length = 1.0; // 焦距，相机中心到viewport（相互正交）的距离
    auto viewport_height = 2.0; // 定义viewport高度为2
    auto viewport_width = viewport_height * (double(image_width) / image_height); // 计算出viewport的宽度
    auto camera_center = point3(0, 0, 0); // 定义相机的中心观察点

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
        - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2; // 计算viewport左上角的坐标
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); // 计算左上角[0][0]处像素的中心点坐标

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::cerr << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world); // 计算被这样的ray照射的pixel的颜色为什么
            write_color(std::cout, pixel_color); // 输出当前像素的颜色
        }
    }

    std::cerr << "\rDone.                 \n";

	return 0;
}