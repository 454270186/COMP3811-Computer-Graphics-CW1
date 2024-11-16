#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Small Triangle (Pixel-Sized)", "[precision]")
{
    Surface surface(320, 240);
    surface.fill({255, 255, 255});

    draw_triangle_interp(surface,
        {160.f, 120.f}, {161.f, 120.f}, {160.f, 121.f}, 
        {1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f});

    auto const col = find_most_red_pixel(surface);
    REQUIRE(255 == int(col.r));
    REQUIRE(255 == int(col.g));
    REQUIRE(255 == int(col.b));
}