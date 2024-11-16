#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Extremely Large Triangle (Crosses Entire Viewport)", "[performance][clipping]")
{
    Surface surface(320, 240);
    surface.fill({255, 255, 255});

    draw_triangle_interp(surface,
        {-100.f, -100.f}, {420.f, -100.f}, {160.f, 500.f}, 
        {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 0.f, 0.f});

    auto const col = find_most_red_pixel(surface);
    REQUIRE(255 == int(col.r));
    REQUIRE(255 == int(col.g));
    REQUIRE(255 == int(col.b));
}

TEST_CASE("Degenerate Triangle", "[edge-case]")
{
    Surface surface(320, 240);
    surface.fill({255, 255, 255});

    draw_triangle_interp(surface,
        {100.f, 100.f}, {200.f, 200.f}, {300.f, 300.f}, 
        {1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f});

    // Ensure the degenerate case doesn't cause crashes or unexpected behavior
    auto const col = find_most_red_pixel(surface);
    REQUIRE(255 == int(col.r));
    REQUIRE(255 == int(col.g));
    REQUIRE(255 == int(col.b));
}