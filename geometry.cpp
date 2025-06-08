#include "geometry.h"
#include <iostream>

Vec3f barycentric(Vec3f p, Vec3f a, Vec3f b, Vec3f c)
{
    float gamma = ((a.y - b.y) * p.x + (b.x - a.x) * p.y + a.x * b.y - b.x * a.y) / 
                  ((a.y - b.y) * c.x + (b.x - a.x) * c.y + a.x * b.y - b.x * a.y);

    float beta = ((a.y - c.y) * p.x + (c.x - a.x) * p.y + a.x * c.y - c.x * a.y) /
                 ((a.y - c.y) * b.x + (c.x - a.x) * b.y + a.x * c.y - c.x * a.y);

    return Vec3f((1 - gamma - beta), beta, gamma);
}

Vec3f barycentric(Vec3i p, Vec3i a, Vec3i b, Vec3i c)
{
    float gamma = 1.0 * ((a.y - b.y) * p.x + (b.x - a.x) * p.y + a.x * b.y - b.x * a.y) /
                        ((a.y - b.y) * c.x + (b.x - a.x) * c.y + a.x * b.y - b.x * a.y);

    float beta = 1.0 * ((a.y - c.y) * p.x + (c.x - a.x) * p.y + a.x * c.y - c.x * a.y) /
                        ((a.y - c.y) * b.x + (c.x - a.x) * b.y + a.x * c.y - c.x * a.y);

    return Vec3f((1 - gamma - beta), beta, gamma);
}

int cross(Vec2i A, Vec2i B)
{
    return A.x* B.y - A.y * B.x;
}

bool same_sign(int a, int b, int c)
{
    return (a * b >= 0) && (b * c >= 0);
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void triangle_cross(Vec2i a, Vec2i b, Vec2i c, TGAImage& framebuffer, TGAColor color) {
    int boundingBoxMinX = std::min(a.x, std::min(b.x, c.x));
    int boundingBoxMinY = std::min(a.y, std::min(b.y, c.y));
    int boundingBoxMaxX = std::max(a.x, std::max(b.x, c.x));
    int boundingBoxMaxY = std::max(a.y, std::max(b.y, c.y));

    //line(boundingBoxMinX, boundingBoxMinY, boundingBoxMaxX, boundingBoxMaxY, framebuffer, color);
    int count = 0;
    //std::cout << boundingBoxMinX << ", " << boundingBoxMinY << ", " << boundingBoxMaxX << ", " << boundingBoxMaxY << std::endl;
    for (int y = boundingBoxMinY; y < boundingBoxMaxY; y++)
    {
        for (int x = boundingBoxMinX; x < boundingBoxMaxX; x++)
        {
            Vec2i AP = Vec2i(x - a.x, y - a.y);
            Vec2i BP = Vec2i(x - b.x, y - b.y);
            Vec2i CP = Vec2i(x - c.x, y - c.y);

            Vec2i AB = Vec2i(b.x - a.x, b.y - a.y);
            Vec2i BC = Vec2i(c.x - b.x, c.y - b.y);
            Vec2i CA = Vec2i(a.x - c.x, a.y - c.y);

            int cross1 = cross(AB, AP);
            int cross2 = cross(BC, BP);
            int cross3 = cross(CA, CP);

            if (same_sign(cross1, cross2, cross3))
            {
                
                count++;
                framebuffer.set(x, y, color);
            }
        }
    }
    //std::cout << count << std::endl;

    //std::cout << (boundingBoxMaxY - boundingBoxMinY) * (boundingBoxMaxX - boundingBoxMinX);
}

void triangle_bc(Vec3i a, Vec3i b, Vec3i c, TGAImage& framebuffer, TGAColor color)
{
    int boundingBoxMinX = std::min(a.x, std::min(b.x, c.x));
    int boundingBoxMinY = std::min(a.y, std::min(b.y, c.y));
    int boundingBoxMaxX = std::max(a.x, std::max(b.x, c.x));
    int boundingBoxMaxY = std::max(a.y, std::max(b.y, c.y));

    for (int y = boundingBoxMinY; y < boundingBoxMaxY; y++)
    {
        for (int x = boundingBoxMinX; x < boundingBoxMaxX; x++)
        {
            Vec3f bc_out = barycentric(Vec3i(x, y, 0), a, b, c);
            Vec3i bc_inside = (a + b + c) * 0.33333;

            Vec3i ai = a + (bc_inside - a) * 0.4;
            Vec3i bi = b + (bc_inside - b) * 0.4;
            Vec3i ci = c + (bc_inside - c) * 0.4;

            Vec3f bc_in = barycentric(Vec3i(x, y, 0), ai, bi, ci);
            if (bc_out.x < 0 || bc_out.y < 0 || bc_out.z < 0) continue;

            if (bc_in.x > 0 && bc_in.y > 0 && bc_in.z > 0) continue;

            //framebuffer.set(x, y, TGAColor(color.r * bc.x, color.g * bc.y, color.b * bc.z, 255)); //colorful triangle
            framebuffer.set(x, y, TGAColor(color.r * bc_out.x, color.g * bc_out.y, color.b * bc_out.z, 255));
            /*float t = bc.x * a.z + bc.y * b.z + bc.z * c.z;
            framebuffer.set(x, y, TGAColor(t, t, t, 255));*/
        }
    }
}

void triangle(Vec3f a, Vec3f b, Vec3f c, float* zbuffer, TGAImage& framebuffer, TGAColor color)
{
    int boundingBoxMinX = std::min(a.x, std::min(b.x, c.x));
    int boundingBoxMinY = std::min(a.y, std::min(b.y, c.y));
    int boundingBoxMaxX = std::max(a.x, std::max(b.x, c.x));
    int boundingBoxMaxY = std::max(a.y, std::max(b.y, c.y));

    for (int y = boundingBoxMinY; y < boundingBoxMaxY; y++)
    {
        for (int x = boundingBoxMinX; x < boundingBoxMaxX; x++)
        {
            Vec3f bc_out = barycentric(Vec3f(x, y, 0), a, b, c);

            if (bc_out.x < 0 || bc_out.y < 0 || bc_out.z < 0) continue;

            float depth = bc_out.x * a.z + bc_out.y * b.z + bc_out.z * c.z;

            int idx = x + y * (boundingBoxMaxX - boundingBoxMinX);

            if (zbuffer[idx] < depth)
            {
                zbuffer[idx] = depth;
                framebuffer.set(x, y, color);
            }  
        }
    }
}

void triangle_z(Vec3f *wps, Vec3i *sps, float* zbuffer, TGAImage& framebuffer, TGAColor color)
{
    int boundingBoxMinX = std::min(sps[0].x, std::min(sps[1].x, sps[2].x));
    int boundingBoxMinY = std::min(sps[0].y, std::min(sps[1].y, sps[2].y));
    int boundingBoxMaxX = std::max(sps[0].x, std::max(sps[1].x, sps[2].x));
    int boundingBoxMaxY = std::max(sps[0].y, std::max(sps[1].y, sps[2].y));

    std::cout << boundingBoxMinX << std::endl;
    std::cout << boundingBoxMaxX << std::endl;
    std::cout << boundingBoxMinY << std::endl;
    std::cout << boundingBoxMaxY << std::endl;

    int count = 0;
    for (int y = boundingBoxMinY; y < boundingBoxMaxY; y++)
    {
        for (int x = boundingBoxMinX; x < boundingBoxMaxX; x++)
        {
            Vec3f bc_out = barycentric(Vec3i(x, y, 0), sps[0], sps[1], sps[2]);
            if (bc_out.x < 0 || bc_out.y < 0 || bc_out.z < 0) continue;

            float depth = bc_out.x * wps[0].z + bc_out.y * wps[1].z + bc_out.z * wps[2].z;

            int idx = x + y * framebuffer.get_width();

            if (zbuffer[idx] < depth)
            {
                count++;
                zbuffer[idx] = depth;
                framebuffer.set(x, y, TGAColor(depth*255, depth * 255, depth * 255, 255));
            }
        }
    }
    std::cout << count << std::endl;
}