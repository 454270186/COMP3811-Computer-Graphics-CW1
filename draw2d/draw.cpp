#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"

void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	int x0 = static_cast<int>(aBegin.x);
	int y0 = static_cast<int>(aBegin.y);
	int x1 = static_cast<int>(aEnd.x);
	int y1 = static_cast<int>(aEnd.y);

	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);

	int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

	while (true) {
		if (x0 >= 0 && x0 < aSurface.get_width() && y0 >= 0 && y0 < aSurface.get_height()) {
            aSurface.set_pixel_srgb(x0, y0, aColor);
        }

		// if reach the end, break
		if (x0 == x1 && y0 == y1) {
			break;
		}

		int e2 = 2 * err;

		// adjust x and y
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
	}
}

void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	// sort vertices by y-coordinate
	if (aP0.y > aP1.y) std::swap(aP0, aP1);
    if (aP1.y > aP2.y) std::swap(aP1, aP2);
    if (aP0.y > aP1.y) std::swap(aP0, aP1);

	auto interpolateX = [](Vec2f p1, Vec2f p2, float y) -> float {
        return p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
    };

	// draw the top part of the triangle
	if (aP0.y != aP1.y) {
        for (float y = aP0.y; y <= aP1.y; ++y) {
            float x_start = interpolateX(aP0, aP2, y);
            float x_end = interpolateX(aP0, aP1, y);

            if (x_start > x_end) std::swap(x_start, x_end);
            for (float x = x_start; x <= x_end; ++x) {
                if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
                    aSurface.set_pixel_srgb(static_cast<int>(x), static_cast<int>(y), aColor);
                }
            }
        }
    }

	// draw the bottom part of the triangle (aP1 to aP2)
	if (aP1.y != aP2.y) {
        for (float y = aP1.y; y <= aP2.y; ++y) {
            float x_start = interpolateX(aP0, aP2, y);
            float x_end = interpolateX(aP1, aP2, y);

            if (x_start > x_end) std::swap(x_start, x_end);
            for (float x = x_start; x <= x_end; ++x) {
                if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
                    aSurface.set_pixel_srgb(static_cast<int>(x), static_cast<int>(y), aColor);
                }
            }
        }
    }
}

void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

    float minX = std::min({ aP0.x, aP1.x, aP2.x });
    float maxX = std::max({ aP0.x, aP1.x, aP2.x });
    float minY = std::min({ aP0.y, aP1.y, aP2.y });
    float maxY = std::max({ aP0.y, aP1.y, aP2.y });

    for (int y = static_cast<int>(minY); y <= static_cast<int>(maxY); ++y) {
        for (int x = static_cast<int>(minX); x <= static_cast<int>(maxX); ++x) {
            float denominator = (aP1.y - aP2.y) * (aP0.x - aP2.x) + (aP2.x - aP1.x) * (aP0.y - aP2.y);
            float lambda0 = ((aP1.y - aP2.y) * (x - aP2.x) + (aP2.x - aP1.x) * (y - aP2.y)) / denominator;
            float lambda1 = ((aP2.y - aP0.y) * (x - aP2.x) + (aP0.x - aP2.x) * (y - aP2.y)) / denominator;
            float lambda2 = 1.0f - lambda0 - lambda1;

            if (lambda0 >= 0 && lambda1 >= 0 && lambda2 >= 0) {
                ColorF color = {
                    lambda0 * aC0.r + lambda1 * aC1.r + lambda2 * aC2.r,
                    lambda0 * aC0.g + lambda1 * aC1.g + lambda2 * aC2.g,
                    lambda0 * aC0.b + lambda1 * aC1.b + lambda2 * aC2.b
                };

                ColorU8_sRGB srgbColor = {
                    linear_to_srgb(color.r),
                    linear_to_srgb(color.g),
                    linear_to_srgb(color.b)
                };

                if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
                    aSurface.set_pixel_srgb(x, y, srgbColor);
                }
            }
        }
    }
}

void draw_line_dda(Surface& aSurface, Vec2f aStart, Vec2f aEnd, ColorU8_sRGB aColor)
{
    float dx = aEnd.x - aStart.x;
    float dy = aEnd.y - aStart.y;

    int steps = std::max(std::abs(dx), std::abs(dy));

    float xIncrement = dx / steps;
    float yIncrement = dy / steps;

    float x = aStart.x;
    float y = aStart.y;

    // Draw each pixel
    for (int i = 0; i <= steps; ++i)
    {
        int ix = static_cast<int>(x);
        int iy = static_cast<int>(y);

        if (ix >= 0 && ix < aSurface.get_width() && iy >= 0 && iy < aSurface.get_height())
        {
            aSurface.set_pixel_srgb(ix, iy, aColor);
        }

        x += xIncrement;
        y += yIncrement;
    }
}

void draw_line_optimized(Surface& aSurface, Vec2f aStart, Vec2f aEnd, ColorU8_sRGB aColor)
{
    // Early clipping to screen boundaries
    auto clip_to_screen = [&aSurface](Vec2f& p1, Vec2f& p2) {
        int w = aSurface.get_width();
        int h = aSurface.get_height();

        // Clip x-coordinates
        if (p1.x < 0) p1.x = 0;
        if (p1.x >= w) p1.x = w - 1;
        if (p2.x < 0) p2.x = 0;
        if (p2.x >= w) p2.x = w - 1;

        // Clip y-coordinates
        if (p1.y < 0) p1.y = 0;
        if (p1.y >= h) p1.y = h - 1;
        if (p2.y < 0) p2.y = 0;
        if (p2.y >= h) p2.y = h - 1;
    };

    clip_to_screen(aStart, aEnd);

    // Standard Bresenham line-drawing logic after clipping
    int x0 = static_cast<int>(aStart.x);
    int y0 = static_cast<int>(aStart.y);
    int x1 = static_cast<int>(aEnd.x);
    int y1 = static_cast<int>(aEnd.y);

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true)
    {
        aSurface.set_pixel_srgb(x0, y0, aColor);

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

void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}