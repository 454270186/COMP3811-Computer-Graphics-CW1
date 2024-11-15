#include "image.hpp"

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>

#include <stb_image.h>

#include "surface.hpp"

#include "../support/error.hpp"

#define STB_IMAGE_IMPLEMENTATION

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );
	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr ) {
		throw Error( "Unable to load image \"%s\"", aPath );
	}

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	
	int startX = static_cast<int>(aPosition.x);
    int startY = static_cast<int>(aPosition.y);

    int imageWidth = aImage.get_width();
    int imageHeight = aImage.get_height();

    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            ColorU8_sRGB_Alpha pixel = aImage.get_pixel(x, y);

            // Check alpha value
            if (pixel.a < 128) {
                continue;
            }

            int destX = startX + x;
            int destY = startY + y;

            // Check bounds to avoid out-of-bounds drawing
            if (destX >= 0 && destX < aSurface.get_width() && destY >= 0 && destY < aSurface.get_height()) {
			    aSurface.set_pixel_srgb(destX, destY, { pixel.r, pixel.g, pixel.b });
            }
        }
    }
}

namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}
