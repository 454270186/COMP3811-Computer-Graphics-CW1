#include <benchmark/benchmark.h>

#include <algorithm>

#include <cassert>

#include "../draw2d/image.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

namespace
{
	void default_blit_earth_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( "assets/earth.png" );
		assert( source );

		for( auto _ : aState )
		{
			// Make sure as much as possible of the image is visible. The
			// sample image here is 1k*1k. Blitting to center = (500,500) makes
			// sure that as much as possible of the image ends up in the target
			// surface.
			blit_masked( surface, *source, {500.f, 500.f} );

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory(); 
		}

		// The following enables the benchmarking library to print information
		// about the memory bandwidth. The total number of bytes processed is
		// *approximatively* two times the total number of bytes in the blit,
		// accounding for both reading and writing. ("Approximatively" since
		// not all pixels are written.)
		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
	}

	void blit_no_alpha_loop_(benchmark::State& aState)
	{
	    auto const width = std::uint32_t(aState.range(0));
	    auto const height = std::uint32_t(aState.range(1));

	    SurfaceEx surface(width, height);
	    surface.clear();

	    auto source = load_image("assets/earth.png");
	    assert(source);

	    for (auto _ : aState)
	    {
	        int startX = 500;
	        int startY = 500;
	        for (int y = 0; y < source->get_height(); ++y)
	        {
	            for (int x = 0; x < source->get_width(); ++x)
	            {
	                int destX = startX + x;
	                int destY = startY + y;

	                if (destX >= 0 && destX < surface.get_width() && destY >= 0 && destY < surface.get_height())
	                {
	                    ColorU8_sRGB_Alpha pixel = source->get_pixel(x, y);
						ColorU8_sRGB aColor = {
    					    pixel.r,
    					    pixel.g,
    					    pixel.b
    					};
	                    surface.set_pixel_srgb(destX, destY, aColor);
	                }
	            }
	        }
	        benchmark::ClobberMemory();
	    }

	    auto const maxBlitX = std::min(width, source->get_width());
	    auto const maxBlitY = std::min(height, source->get_height());

	    aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	}

	void blit_no_alpha_memcpy_(benchmark::State& aState)
	{
	    auto const width = std::uint32_t(aState.range(0));
	    auto const height = std::uint32_t(aState.range(1));

	    SurfaceEx surface(width, height);
	    surface.clear();

	    auto source = load_image("assets/earth.png");
	    assert(source);

	    for (auto _ : aState)
	    {
	        int startY = 500; // Starting position on the target surface
	        int maxHeight = std::min(source->get_height(), height - startY);

	        for (int y = 0; y < maxHeight; ++y)
	        {
	            int destY = startY + y;
	            if (destY >= 0 && destY < surface.get_height())
	            {
	                // Use manual pixel copying instead of get_line
	                for (int x = 0; x < source->get_width(); ++x)
	                {
	                    int destX = x;
	                    if (destX >= 0 && destX < surface.get_width())
	                    {
	                        auto color = source->get_pixel(x, y);
	                        surface.set_pixel_srgb(destX, destY, {color.r, color.g, color.b});
	                    }
	                }
	            }
	        }
	        benchmark::ClobberMemory();
	    }

	    auto const maxBlitX = std::min(width, source->get_width());
	    auto const maxBlitY = std::min(height, source->get_height());

	    aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	}


		// void my_other_blit_( benchmark::State& aState )
		// {
		// 		...
		// }
}

BENCHMARK( default_blit_earth_ )
	->Args({320, 240, 128})  // Small framebuffer, 128x128 image
    ->Args({320, 240, 1024}) // Small framebuffer, 1024x1024 image
    ->Args({1280, 720, 128}) // Default framebuffer, 128x128 image
    ->Args({1280, 720, 1024}) // Default framebuffer, 1024x1024 image
    ->Args({1920, 1080, 128}) // Full HD framebuffer, 128x128 image
    ->Args({1920, 1080, 1024}) // Full HD framebuffer, 1024x1024 image
    ->Args({7680, 4320, 128}) // 8k framebuffer, 128x128 image
    ->Args({7680, 4320, 1024}); // 8k framebuffer, 1024x1024 image

BENCHMARK(blit_no_alpha_loop_)
    ->Args({320, 240, 128})  // Small framebuffer, 128x128 image
    ->Args({320, 240, 1024}) // Small framebuffer, 1024x1024 image
    ->Args({1280, 720, 128}) // Default framebuffer, 128x128 image
    ->Args({1280, 720, 1024}) // Default framebuffer, 1024x1024 image
    ->Args({1920, 1080, 128}) // Full HD framebuffer, 128x128 image
    ->Args({1920, 1080, 1024}) // Full HD framebuffer, 1024x1024 image
    ->Args({7680, 4320, 128}) // 8k framebuffer, 128x128 image
    ->Args({7680, 4320, 1024}); // 8k framebuffer, 1024x1024 image

BENCHMARK(blit_no_alpha_memcpy_)
    ->Args({320, 240, 128})  // Small framebuffer, 128x128 image
    ->Args({320, 240, 1024}) // Small framebuffer, 1024x1024 image
    ->Args({1280, 720, 128}) // Default framebuffer, 128x128 image
    ->Args({1280, 720, 1024}) // Default framebuffer, 1024x1024 image
    ->Args({1920, 1080, 128}) // Full HD framebuffer, 128x128 image
    ->Args({1920, 1080, 1024}) // Full HD framebuffer, 1024x1024 image
    ->Args({7680, 4320, 128}) // 8k framebuffer, 128x128 image
    ->Args({7680, 4320, 1024}); // 8k framebuffer, 1024x1024 image

//BENCHMARK( my_other_blit_ )
//	->Args( { 320, 240 } )
//	->Args( { 7680, 4320 } )
//;

BENCHMARK_MAIN();
