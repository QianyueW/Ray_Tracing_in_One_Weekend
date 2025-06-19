#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "rtweekend.h"

class hittable_list : public hittable {
public:
    // ������һ��������Ա���� objects������һ�� std::vector ���ͣ����ڴ洢ָ�� hittable ����Ĺ���ָ�롣
    // ʹ�� shared_ptr �Ƿǳ���Ҫ�ģ���Ϊ�������ڶ���ط�����ͬһ������� ownership������Ȩ������û�� shared_ptr ��ָ��ö���ʱ��������Զ���ɾ������Ч�������ڴ档
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
        auto closest_so_far = ray_t.max; // ��ʼ��һ����������ʾĿǰ�ҵ����������� t ֵ�����ĳ�ʼֵ�� ray_tmax������ζ���κα� ray_tmax С�Ľ��㶼�������ǡ�

        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min,closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t; // ͨ�������ҵ�������Ǹ�����Ľ���
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif