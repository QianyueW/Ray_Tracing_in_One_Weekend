#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "rtweekend.h"

class hittable_list : public hittable {
public:
    // 声明了一个公共成员变量 objects，它是一个 std::vector 类型，用于存储指向 hittable 对象的共享指针。
    // 使用 shared_ptr 是非常重要的，因为它允许在多个地方共享同一个对象的 ownership（所有权），当没有 shared_ptr 再指向该对象时，对象会自动被删除，有效管理了内存。
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max; // 初始化一个变量，表示目前找到的最近交点的 t 值。它的初始值是 ray_tmax，这意味着任何比 ray_tmax 小的交点都将被考虑。

        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min,closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t; // 通过遍历找到最近的那个物体的交点
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif