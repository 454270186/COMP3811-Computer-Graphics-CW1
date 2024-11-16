#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

//TODO
TEST_CASE("Partially offscreen vertical line", "[clip]") {
    Surface surface(640, 480);
    surface.clear();

    draw_line_solid(surface,
        {320.f, -100.f},
        {320.f, 240.f},
        {255, 255, 255}
    );

    auto const pixels = max_col_pixel_count(surface);
    REQUIRE(pixels > 0);
}
