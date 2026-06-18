#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

unsigned char image[HEIGHT][WIDTH][3];

void setPixel(int x, int y, int r, int g, int b)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        image[y][x][0] = r;
        image[y][x][1] = g;
        image[y][x][2] = b;
    }
}

void drawCircle(int cx, int cy, int radius, int r, int g, int b)
{
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            if (x * x + y * y <= radius * radius)
            {
                setPixel(cx + x, cy + y, r, g, b);
            }
        }
    }
}

void drawLine(int x0, int y0, int x1, int y1, int r, int g, int b)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int err = dx - dy;

    while (1)
    {
        setPixel(x0, y0, r, g, b);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

int main()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            image[y][x][0] = 10;
            image[y][x][1] = 10;
            image[y][x][2] = 30;
        }
    }

    int x0 = 200, y0 = 100;
    int x1 = 600, y1 = 220;
    int x2 = 300, y2 = 500;
    int x3 = 700, y3 = 580;

    drawLine(x0, y0, x1, y1, 100, 100, 100);
    drawLine(x2, y2, x3, y3, 100, 100, 100);

    for (double t = 0; t <= 1.0; t += 0.001)
    {
        double u = 1.0 - t;

        double x =
            u * u * u * x0 +
            3 * u * u * t * x1 +
            3 * u * t * t * x2 +
            t * t * t * x3;

        double y =
            u * u * u * y0 +
            3 * u * u * t * y1 +
            3 * u * t * t * y2 +
            t * t * t * y3;

        drawCircle((int)x, (int)y, 2, 255, 80, 20);
    }

    drawCircle(x0, y0, 6, 255, 255, 255);
    drawCircle(x3, y3, 6, 255, 255, 255);

    drawCircle(x1, y1, 6, 255, 220, 0);
    drawCircle(x2, y2, 6, 255, 220, 0);

    FILE *f = fopen("render.ppm", "wb");
    if (f == NULL) {
        return 1;
    }

    fprintf(f, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            fprintf(f, "%d %d %d ",
                    image[y][x][0],
                    image[y][x][1],
                    image[y][x][2]);
        }
        fprintf(f, "\n");
    }

    fclose(f);

    return 0;
}