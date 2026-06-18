#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH  800
#define HEIGHT 600
#define SEGMENTS 100

typedef struct {
    float x, y;
} Point;

typedef struct {
    float r, g, b;
} Color;

typedef struct {
    Point p0, p1, p2, p3;
    Color color;
} BezierCurve;

Point bezier(float t, BezierCurve c) {
    float u = 1.0f - t;

    Point p;
    p.x = u*u*u*c.p0.x + 3*u*u*t*c.p1.x + 3*u*t*t*c.p2.x + t*t*t*c.p3.x;
    p.y = u*u*u*c.p0.y + 3*u*u*t*c.p1.y + 3*u*t*t*c.p2.y + t*t*t*c.p3.y;

    return p;
}

int clamp(int v) {
    if (v < 0) return 0;
    if (v > 255) return 255;
    return v;
}

int main() {
    FILE *f = fopen("render.ppm", "w");
    if (!f) {
        printf("Failed to open file\n");
        return 1;
    }

    Color bg = {0.05f, 0.05f, 0.1f};

    BezierCurve curve;
    curve.p0 = (Point){100, 500};
    curve.p1 = (Point){200, 100};
    curve.p2 = (Point){600, 100};
    curve.p3 = (Point){700, 500};

    curve.color = (Color){0.2f, 1.0f, 0.4f};

    fprintf(f, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            float fx = (float)x;
            float fy = (float)y;

            float minDist = 1e9f;

            for (int i = 0; i <= SEGMENTS; i++) {
                float t = (float)i / SEGMENTS;
                Point p = bezier(t, curve);

                float dx = fx - p.x;
                float dy = fy - p.y;

                float d = sqrtf(dx*dx + dy*dy);

                if (d < minDist)
                    minDist = d;
            }

            float intensity = expf(-(minDist * minDist) / 200.0f);

            float r = bg.r * (1 - intensity) + curve.color.r * intensity;
            float g = bg.g * (1 - intensity) + curve.color.g * intensity;
            float b = bg.b * (1 - intensity) + curve.color.b * intensity;

            int ir = clamp((int)(r * 255));
            int ig = clamp((int)(g * 255));
            int ib = clamp((int)(b * 255));

            fprintf(f, "%d %d %d ", ir, ig, ib);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return 0;
}
