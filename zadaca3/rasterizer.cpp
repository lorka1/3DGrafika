#include <iostream>
#include <cmath>
#include "tgaimage.h"
#include "tgaimage.cpp"
using namespace std;

// compile with:
// g++ rasterizer.cpp tgaimage.cpp -o out.exe

// dimenzije slike
const int width = 512;
const int height = 512;

void set_color(int x, int y, TGAImage &image, TGAColor color, bool invert = false)
{
    image.set(y, x, color);
}

float line(float x0, float y0, float x1, float y1, float x, float y)
{
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

void line_naive(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    for (float t = 0; t < 1; t += 0.01)
    {
        int x = x0 * (1.0f - t) + x1 * t;
        int y = x0 * (1.0f - t) + y1 * t;
        set_color(x, y, image, color);
    }
}

void draw_triangle_2d(float x0, float y0, float x1, float y1, float x2, float y2, TGAImage &image, TGAColor color)
{
    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float ymin = min(min(floor(y0), floor(y1)), floor(y2));

    float xmax = max(max(ceil(x0), floor(x1)), floor(x2));
    float ymax = max(max(ceil(y0), floor(y1)), floor(y2));

    for (float y = ymin; y <= ymax; y++)
    {
        for (float x = xmin; x <= xmax; x++)
        {
            float alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            float beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            float gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1)
            {
                set_color(x, y, image, color);
            }
        }
    }
}

void draw_triangle_2d_gouraud(float x0, float y0, float x1, float y1, float x2, float y2, TGAImage &image, TGAColor c0, TGAColor c1, TGAColor c2)
{
    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float ymin = min(min(floor(y0), floor(y1)), floor(y2));

    float xmax = max(max(ceil(x0), floor(x1)), floor(x2));
    float ymax = max(max(ceil(y0), floor(y1)), floor(y2));

    for (float y = ymin; y <= ymax; y++)
    {
        for (float x = xmin; x <= xmax; x++)
        {
            float alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            float beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            float gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);
            // TGAColor = alpha*c0 + beta*c1 + gamma*c2;

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1)
            {
                set_color(x, y, image, TGAColor((alpha * c0.r + beta * c1.r + gamma * c2.r), (alpha * c0.g + beta * c1.g + gamma * c2.g), (alpha * c0.b + beta * c1.b + gamma * c2.b), 255));
            }
        }
    }
}

void draw_triangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, TGAImage &image, TGAColor color)
{
    x0 = x0 / z0;
    y0 = y0 / z0;
    x1 = x1 / z1;
    y1 = y1 / z1;
    x2 = x2 / z2;
    y2 = y2 / z2;

    x0 = (1 + x0) * 0.5 * width;
    y0 = (1 + y0) * 0.5 * height;

    x1 = (1 + x1) * 0.5 * width;
    y1 = (1 + y1) * 0.5 * height;

    x2 = (1 + x2) * 0.5 * width;
    y2 = (1 + y2) * 0.5 * height;

    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float ymin = min(min(floor(y0), floor(y1)), floor(y2));

    float xmax = max(max(ceil(x0), floor(x1)), floor(x2));
    float ymax = max(max(ceil(y0), floor(y1)), floor(y2));

    for (float y = ymin; y <= ymax; y++)
    {
        for (float x = xmin; x <= xmax; x++)
        {
            float alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            float beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            float gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1)
            {
                set_color(x, y, image, color);
            }
        }
    }
}

void draw_triangle_tex(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float u0, float v0, float u1, float v1, float u2, float v2, const char* path,  TGAImage &image){
    TGAImage tex;
    tex.read_tga_file(path);

    x0 = x0 / z0;
    y0 = y0 / z0;
    x1 = x1 / z1;
    y1 = y1 / z1;
    x2 = x2 / z2;
    y2 = y2 / z2;

    x0 = (1 + x0) * 0.5 * width;
    y0 = (1 + y0) * 0.5 * height;

    x1 = (1 + x1) * 0.5 * width;
    y1 = (1 + y1) * 0.5 * height;

    x2 = (1 + x2) * 0.5 * width;
    y2 = (1 + y2) * 0.5 * height;

    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float ymin = min(min(floor(y0), floor(y1)), floor(y2));

    float xmax = max(max(ceil(x0), floor(x1)), floor(x2));
    float ymax = max(max(ceil(y0), floor(y1)), floor(y2));

     for (float y = ymin; y <= ymax; y++)
    {
        for (float x = xmin; x <= xmax; x++)
        {
            float alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            float beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            float gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1)
            {
                int Us = round(tex.get_width() * (alpha * u0 + beta * u1 + gamma * u2));
                int Vs = round(tex.get_height() * (alpha * v0 + beta * v1 + gamma * v2));
                set_color(x, y, image, tex.get(Us, Vs));
            }
        }
    }
}

void draw_triangle_tex_corrected(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float u0, float v0, float u1, float v1, float u2, float v2, const char* path,  TGAImage &image){
    TGAImage tex;
    tex.read_tga_file(path);

    x0 = x0 / z0;
    y0 = y0 / z0;
    u0 = u0 / z0;
    v0 = v0 / z0;
    x1 = x1 / z1;
    y1 = y1 / z1;
    u1 = u1 / z1;
    v1 = v1 / z1;
    x2 = x2 / z2;
    y2 = y2 / z2;
    u2 = u2 / z2;
    v2 = v2 / z2;

    z0 = 1 / z0;
    z1 = 1 / z1;
    z2 = 1 / z2;


    x0 = (1 + x0) * 0.5 * width;
    y0 = (1 + y0) * 0.5 * height;

    x1 = (1 + x1) * 0.5 * width;
    y1 = (1 + y1) * 0.5 * height;

    x2 = (1 + x2) * 0.5 * width;
    y2 = (1 + y2) * 0.5 * height;

    float xmin = min(min(floor(x0), floor(x1)), floor(x2));
    float ymin = min(min(floor(y0), floor(y1)), floor(y2));

    float xmax = max(max(ceil(x0), floor(x1)), floor(x2));
    float ymax = max(max(ceil(y0), floor(y1)), floor(y2));

     for (float y = ymin; y <= ymax; y++)
    {
        for (float x = xmin; x <= xmax; x++)
        {
            float alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            float beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            float gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);

            if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1)
            {
                float Us = tex.get_width() * (alpha * u0 + beta * u1 + gamma * u2);
                float Vs = tex.get_height() * (alpha * v0 + beta * v1 + gamma * v2);
                float z = 1 / (alpha * z0 + beta * z1 + gamma * z2);
                Us = round(Us * z);
                Vs = round(Vs * z);
                set_color(x, y, image, tex.get(Us, Vs));
            }
        }
    }
}

void line_midpoint(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color, bool invert)
{
    int y = y0;
    int d = line(x0, y0, x1, y1, x0 + 1, y0 + 0.5);

    int dx = (x1 - x0);
    int dy = (y1 - y0);
    int increment = 1;

    if (dy < 0)
    {
        // pravac ide od gore prema dolje
        dy = -dy;
        increment = -increment;
    }

    for (int x = x0; x <= x1; ++x)
    {
        if (invert)
        {
            set_color(x, y, image, color);
        }
        else
        {
            set_color(y, x, image, color);
        }

        if (d < 0)
        {
            y = y + increment;
            d = d + dx - dy;
        }
        else
        {
            d = d - dy;
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // 'transponiraj' duzinu ako je veci od 1
    bool invert = false;
    if (abs(x1 - x0) < abs(y1 - y0))
    {
        swap(x0, y0);
        swap(x1, y1);
        invert = true;
    }

    // zamijeni tocke ako pravac ide s desna na lijevo
    if (x1 < x0)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    // nacrtaj duzinu
    line_midpoint(x0, y0, x1, y1, image, color, invert);
}

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);

    // nacrtaj nekoliko duzina
    draw_line(10, 20, 180, 60, image, white);
    draw_line(180, 80, 10, 40, image, white);

    draw_line(20, 180, 140, 170, image, green);

    draw_line(80, 40, 110, 120, image, blue);

    // spremi sliku
    image.flip_vertically();
    image.write_tga_file("lines.tga");

    TGAImage image_1(width, height, TGAImage::RGB);

    draw_triangle_2d(20, 330, 200, 450, 300, 130, image_1, red);
    draw_triangle_2d(180, 480, 320, 330, 250, 500, image_1, blue);
    draw_triangle_2d_gouraud(90, 40, 120, 120, 50, 300, image_1, red, blue, green);

    image_1.flip_vertically();
    image_1.write_tga_file("triangle.tga");

    TGAImage image_2(width, height, TGAImage::RGB);

    draw_triangle(-48, -10, 82, 29, -15, 44, 13, 34, 114, image_2, blue);

    image_2.flip_vertically();
    image_2.write_tga_file("triangle_zad3.tga");

    TGAImage image_3(width, height, TGAImage::RGB);
    
    draw_triangle_tex(-48, -10, 82, 29, -15, 44, 13, 34, 114, 0, 0, 0, 1, 1, 0, "tex.tga", image_3);

    image_3.flip_vertically();
    image_3.write_tga_file("triangle_tex_zad4.tga");

    TGAImage image_4(width, height, TGAImage::RGB);

    draw_triangle_tex_corrected(-48, -10, 82, 29, -15, 44, 13, 34, 114, 0, 0, 0, 1, 1, 0, "tex.tga", image_4);

    image_4.flip_vertically();
    image_4.write_tga_file("triangle_tex_corr_zad4.tga");
}