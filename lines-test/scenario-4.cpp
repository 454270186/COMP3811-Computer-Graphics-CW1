#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

//TODO
TEST_CASE("Single pixel line", "[single_pixel]") {
    Surface surface(640, 480);
    surface.clear();

    draw_line_solid(surface,
        {320.f, 240.f},
        {320.f, 240.f},
        {255, 255, 255}
    );

    auto const pixels = max_col_pixel_count(surface);
    REQUIRE(1 == pixels);
}
