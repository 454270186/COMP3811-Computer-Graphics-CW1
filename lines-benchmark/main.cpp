#include <benchmark/benchmark.h>

#include "../draw2d/draw.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

namespace
{
	// This is a placeholder. Replace this with yor own code. Refer to
	// blit-benchmark/main.cpp for a more complete example. 
	void placeholder_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
			// Placeholder that just does something:
			surface.clear(); // PLACEHOLDER! EXCLUDE FROM REAL BENCHMARKS!

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory(); 
		}
	}

	void bresenham_benchmark_(benchmark::State& aState)
    {
        auto const width = std::uint32_t(aState.range(0));
        auto const height = std::uint32_t(aState.range(1));

        SurfaceEx surface(width, height);
        surface.clear();

        for (auto _ : aState)
        {
            draw_line_solid(surface, {10.f, 10.f}, {width - 10.f, height - 10.f}, {255, 255, 255});
            benchmark::ClobberMemory();
        }
    }

	void dda_benchmark_(benchmark::State& aState)
    {
        auto const width = std::uint32_t(aState.range(0));
        auto const height = std::uint32_t(aState.range(1));

        SurfaceEx surface(width, height);
        surface.clear();

        for (auto _ : aState)
        {
            draw_line_dda(surface, {10.f, 10.f}, {width - 10.f, height - 10.f}, {255, 255, 255});
            benchmark::ClobberMemory();
        }
    }
}

void optimized_benchmark_(benchmark::State& aState)
{
    auto const width = std::uint32_t(aState.range(0));
    auto const height = std::uint32_t(aState.range(1));

    SurfaceEx surface(width, height);
    surface.clear();

    for (auto _ : aState)
    {
        draw_line_optimized(surface, {10.f, 10.f}, {width - 10.f, height - 10.f}, {255, 0, 0});
        benchmark::ClobberMemory();
    }
}


BENCHMARK( placeholder_ )
	->Args( { 1920, 1080 } )
	->Args( { 7680, 4320 } )
;

BENCHMARK(bresenham_benchmark_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK(dda_benchmark_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK(optimized_benchmark_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK_MAIN();
