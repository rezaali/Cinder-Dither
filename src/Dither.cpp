#include "Dither.h"

using namespace ci;

namespace reza {
namespace dither {
    
namespace {
    const ColorA white = ColorA( 1.0, 1.0, 1.0, 0.0 );
    const ColorA whiteColor = ColorA( 1.0, 1.0, 1.0, 1.0 );
    const ColorA red = ColorA( 1.0, 0.0, 0.0, 0.0 );
    const ColorA redColor = ColorA( 1.0, 0.0, 0.0, 1.0 );
    const ColorA green = ColorA( 0.0, 1.0, 0.0, 0.0 );
    const ColorA greenColor = ColorA( 0.0, 1.0, 0.0, 1.0 );
    const ColorA blue = ColorA( 0.0, 0.0, 1.0, 0.0 );
    const ColorA blueColor = ColorA( 0.0, 0.0, 1.0, 1.0 );
    const ColorA black = ColorA( 0.0, 0.0, 0.0, 0.0 );
    const ColorA blackColor = ColorA( 0.0, 0.0, 0.0, 1.0 );
}

Surface32fRef linear( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );

    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float whiteDist = length( total - white );
            float blackDist = length( total - black );
            
            if( whiteDist <= blackDist ) {
                color.set( CM_RGB, whiteColor );
                error = total - whiteColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }

            if( x < ( width - 1 ) ) {
                auto posRgt = ivec2( x + 1, y );
                auto pxlRgt = output->getPixel( posRgt );
                output->setPixel( posRgt, pxlRgt + error );
            }

            output->setPixel( pos, color );
        }
    }

    return output;
}

Surface32fRef linearRGB( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );
    
    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float redDist = length( total - red );
            float greenDist = length( total - green );
            float blueDist = length( total - blue );
            float blackDist = length( total - black );
            
            if( redDist <= greenDist && redDist <= blueDist && redDist <= blackDist ) {
                color.set( CM_RGB, redColor );
                error = total - redColor;
            }
            else if( greenDist <= redDist && greenDist <= blueDist && greenDist <= blackDist ) {
                color.set( CM_RGB, greenColor );
                error = total - greenColor;
            }
            else if( blueDist <= redDist && blueDist <= greenDist && blueDist <= blackDist ) {
                color.set( CM_RGB, blueColor );
                error = total - blueColor;
            }
            else if( blackDist <= redDist && blackDist <= greenDist && blackDist <= blueDist ) {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor - redColor - blueColor - greenColor;
            }
            
            if( x < ( width - 1 ) ) {
                auto posRgt = ivec2( x + 1, y );
                auto pxlRgt = output->getPixel( posRgt );
                output->setPixel( posRgt, pxlRgt + error );
            }
            
            output->setPixel( pos, color );
        }
    }
    
    return output;
}

//  FloydSteinberg (1/16)
//      X   7
//  3   5   1
Surface32fRef FloydSteinberg( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );

    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float whiteDist = length( total - white );
            float blackDist = length( total - black );
            
            if( whiteDist <= blackDist ) {
                color.set( CM_RGB, whiteColor );
                error = total - whiteColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }

            error /= 16.0;

            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 7.0f );
            }

            if( y < ( height - 1 ) ) {
                if( ( x - 1 ) >= 0 ) {
                    auto posLeft = ivec2( x - 1, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error * 3.0f );
                }

                auto posCen = ivec2( x, y + 1 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error * 5.0f );

                if( x < ( width - 1 ) ) {
                    auto posRgt = ivec2( x + 1, y + 1 );
                    auto pxlRgt = output->getPixel( posRgt );
                    output->setPixel( posRgt, pxlRgt + error * 1.0f );
                }
            }

            output->setPixel( pos, color );
        }
    }

    return output;
}

//  FloydSteinbergRGB (1/16)
//      X   7
//  3   5   1
Surface32fRef FloydSteinbergRGB( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );

    int width = input->getWidth();
    int height = input->getHeight();

    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;

            float redDist = length( total - red );
            float greenDist = length( total - green );
            float blueDist = length( total - blue );
            float blackDist = length( total - black );

            if( redDist <= greenDist && redDist <= blueDist && redDist <= blackDist ) {
                color.set( CM_RGB, redColor );
                error = total - redColor;
            }
            else if( greenDist <= redDist && greenDist <= blueDist && greenDist <= blackDist ) {
                color.set( CM_RGB, greenColor );
                error = total - greenColor;
                ;
            }
            else if( blueDist <= redDist && blueDist <= greenDist && blueDist <= blackDist ) {
                color.set( CM_RGB, blueColor );
                error = total - blueColor;
            }
            else if( blackDist <= redDist && blackDist <= greenDist && blackDist <= blueDist ) {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor - redColor - blueColor - greenColor;
            }

            error /= 16.0;

            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 7.0f );
            }

            if( y < ( height - 1 ) ) {
                if( ( x - 1 ) >= 0 ) {
                    auto posLeft = ivec2( x - 1, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error * 3.0f );
                }

                auto posCen = ivec2( x, y + 1 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error * 5.0f );

                if( x < ( width - 1 ) ) {
                    auto posRgt = ivec2( x + 1, y + 1 );
                    auto pxlRgt = output->getPixel( posRgt );
                    output->setPixel( posRgt, pxlRgt + error * 1.0f );
                }
            }

            output->setPixel( pos, color );
        }
    }

    return output;
}

// JarvisJudiceNinke (1/48)
//          X   7   5
//  3   5   7   5   3
//  1   3   5   3   1
Surface32fRef JarvisJudiceNinke( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );

    int width = input->getWidth();
    int height = input->getHeight();

    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float whiteDist = length( total - white );
            float blackDist = length( total - black );
            
            if( whiteDist <= blackDist ) {
                color.set( CM_RGB, whiteColor );
                error = total - whiteColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }

            error /= 48.0;

            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 7.0f );
            }

            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 5.0f );
            }

            for( int i = 1; i < 3; i++ ) {
                float offset = ( i - 1 ) * 2.0f;
                if( y < ( height - i ) ) {
                    if( ( x - 1 ) >= 0 ) {
                        auto posLeft = ivec2( x - 1, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 5.0f - offset ) );
                    }

                    if( ( x - 2 ) >= 0 ) {
                        auto posLeft = ivec2( x - 2, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 3.0f - offset ) );
                    }

                    auto posCen = ivec2( x, y + i );
                    auto pxlCen = output->getPixel( posCen );
                    output->setPixel( posCen, pxlCen + error * ( 7.0f - offset ) );

                    if( x < ( width - 1 ) ) {
                        auto posRgt = ivec2( x + 1, y + i );
                        auto pxlRgt = output->getPixel( posRgt );
                        output->setPixel( posRgt, pxlRgt + error * ( 5.0f - offset ) );
                    }

                    if( x < ( width - 2 ) ) {
                        auto posRgtRgt = ivec2( x + 2, y + i );
                        auto pxlRgtRgt = output->getPixel( posRgtRgt );
                        output->setPixel( posRgtRgt, pxlRgtRgt + error * ( 3.0f - offset ) );
                    }
                }
            }
            output->setPixel( pos, color );
        }
    }

    return output;
}

// JarvisJudiceNinke (1/48)
//          X   7   5
//  3   5   7   5   3
//  1   3   5   3   1
Surface32fRef JarvisJudiceNinkeRGB( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );
    
    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float redDist = length( total - red );
            float greenDist = length( total - green );
            float blueDist = length( total - blue );
            float blackDist = length( total - black );
            
            if( redDist <= greenDist && redDist <= blueDist && redDist <= blackDist ) {
                color.set( CM_RGB, redColor );
                error = total - redColor;
            }
            else if( greenDist <= redDist && greenDist <= blueDist && greenDist <= blackDist ) {
                color.set( CM_RGB, greenColor );
                error = total - greenColor;
                ;
            }
            else if( blueDist <= redDist && blueDist <= greenDist && blueDist <= blackDist ) {
                color.set( CM_RGB, blueColor );
                error = total - blueColor;
            }
            else if( blackDist <= redDist && blackDist <= greenDist && blackDist <= blueDist ) {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor - redColor - blueColor - greenColor;
            }
            
            error /= 48.0;
            
            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 7.0f );
            }
            
            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 5.0f );
            }
            
            for( int i = 1; i < 3; i++ ) {
                float offset = ( i - 1 ) * 2.0f;
                if( y < ( height - i ) ) {
                    if( ( x - 1 ) >= 0 ) {
                        auto posLeft = ivec2( x - 1, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 5.0f - offset ) );
                    }
                    
                    if( ( x - 2 ) >= 0 ) {
                        auto posLeft = ivec2( x - 2, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 3.0f - offset ) );
                    }
                    
                    auto posCen = ivec2( x, y + i );
                    auto pxlCen = output->getPixel( posCen );
                    output->setPixel( posCen, pxlCen + error * ( 7.0f - offset ) );
                    
                    if( x < ( width - 1 ) ) {
                        auto posRgt = ivec2( x + 1, y + i );
                        auto pxlRgt = output->getPixel( posRgt );
                        output->setPixel( posRgt, pxlRgt + error * ( 5.0f - offset ) );
                    }
                    
                    if( x < ( width - 2 ) ) {
                        auto posRgtRgt = ivec2( x + 2, y + i );
                        auto pxlRgtRgt = output->getPixel( posRgtRgt );
                        output->setPixel( posRgtRgt, pxlRgtRgt + error * ( 3.0f - offset ) );
                    }
                }
            }
            output->setPixel( pos, color );
        }
    }
    
    return output;
}

//  Stucki (1/42)
//          X   8   4
//  2   4   8   4   2
//  1   2   4   2   1
Surface32fRef Stucki( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );

    int width = input->getWidth();
    int height = input->getHeight();

    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float whiteDist = length( total - white );
            float blackDist = length( total - black );
            
            if( whiteDist <= blackDist ) {
                color.set( CM_RGB, whiteColor );
                error = total - whiteColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }

            error /= 42.0;

            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 8.0f );
            }

            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 4.0f );
            }

            for( int i = 1; i < 3; i++ ) {
                float offset = i;
                if( y < ( height - i ) ) {
                    if( ( x - 1 ) >= 0 ) {
                        auto posLeft = ivec2( x - 1, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 4.0f / offset ) );
                    }

                    if( ( x - 2 ) >= 0 ) {
                        auto posLeft = ivec2( x - 2, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 2.0f / offset ) );
                    }

                    auto posCen = ivec2( x, y + i );
                    auto pxlCen = output->getPixel( posCen );
                    output->setPixel( posCen, pxlCen + error * ( 8.0f / offset ) );

                    if( x < ( width - 1 ) ) {
                        auto posRgt = ivec2( x + 1, y + i );
                        auto pxlRgt = output->getPixel( posRgt );
                        output->setPixel( posRgt, pxlRgt + error * ( 4.0f / offset ) );
                    }

                    if( x < ( width - 2 ) ) {
                        auto posRgtRgt = ivec2( x + 2, y + i );
                        auto pxlRgtRgt = output->getPixel( posRgtRgt );
                        output->setPixel( posRgtRgt, pxlRgtRgt + error * ( 2.0f / offset ) );
                    }
                }
            }
            output->setPixel( pos, color );
        }
    }

    return output;
}
    
//  Stucki (1/42)
//          X   8   4
//  2   4   8   4   2
//  1   2   4   2   1
Surface32fRef StuckiRGB( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );
    
    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float redDist = length( total - red );
            float greenDist = length( total - green );
            float blueDist = length( total - blue );
            float blackDist = length( total - black );
            
            if( redDist <= greenDist && redDist <= blueDist && redDist <= blackDist ) {
                color.set( CM_RGB, redColor );
                error = total - redColor;
            }
            else if( greenDist <= redDist && greenDist <= blueDist && greenDist <= blackDist ) {
                color.set( CM_RGB, greenColor );
                error = total - greenColor;
                ;
            }
            else if( blueDist <= redDist && blueDist <= greenDist && blueDist <= blackDist ) {
                color.set( CM_RGB, blueColor );
                error = total - blueColor;
            }
            else if( blackDist <= redDist && blackDist <= greenDist && blackDist <= blueDist ) {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor - redColor - blueColor - greenColor;
            }
            
            error /= 42.0;
            
            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 8.0f );
            }
            
            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 4.0f );
            }
            
            for( int i = 1; i < 3; i++ ) {
                float offset = i;
                if( y < ( height - i ) ) {
                    if( ( x - 1 ) >= 0 ) {
                        auto posLeft = ivec2( x - 1, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 4.0f / offset ) );
                    }
                    
                    if( ( x - 2 ) >= 0 ) {
                        auto posLeft = ivec2( x - 2, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 2.0f / offset ) );
                    }
                    
                    auto posCen = ivec2( x, y + i );
                    auto pxlCen = output->getPixel( posCen );
                    output->setPixel( posCen, pxlCen + error * ( 8.0f / offset ) );
                    
                    if( x < ( width - 1 ) ) {
                        auto posRgt = ivec2( x + 1, y + i );
                        auto pxlRgt = output->getPixel( posRgt );
                        output->setPixel( posRgt, pxlRgt + error * ( 4.0f / offset ) );
                    }
                    
                    if( x < ( width - 2 ) ) {
                        auto posRgtRgt = ivec2( x + 2, y + i );
                        auto pxlRgtRgt = output->getPixel( posRgtRgt );
                        output->setPixel( posRgtRgt, pxlRgtRgt + error * ( 2.0f / offset ) );
                    }
                }
            }
            output->setPixel( pos, color );
        }
    }
    
    return output;
}

//  Atkinson (1/8)
//          X   1   1
//      1   1   1
//          1
Surface32fRef Atkinson( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );

    int width = input->getWidth();
    int height = input->getHeight();

    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float whiteDist = length( total - white );
            float blackDist = length( total - black );
            
            if( whiteDist <= blackDist ) {
                color.set( CM_RGB, whiteColor );
                error = total - whiteColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }

            error /= 8.0;

            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error );
            }

            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error );
            }

            if( y < ( height - 1 ) ) {
                if( ( x - 1 ) >= 0 ) {
                    auto posLeft = ivec2( x - 1, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error );
                }

                auto posCen = ivec2( x, y + 1 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error );

                if( x < ( width - 1 ) ) {
                    auto posRgt = ivec2( x + 1, y + 1 );
                    auto pxlRgt = output->getPixel( posRgt );
                    output->setPixel( posRgt, pxlRgt + error );
                }
            }

            if( y < ( height - 2 ) ) {
                auto posCen = ivec2( x, y + 2 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error );
            }

            output->setPixel( pos, color );
        }
    }

    return output;
}
    
//  Atkinson (1/8)
//          X   1   1
//      1   1   1
//          1
Surface32fRef AtkinsonRGB( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );
    
    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float redDist = length( total - red );
            float greenDist = length( total - green );
            float blueDist = length( total - blue );
            float blackDist = length( total - black );
            
            if( redDist <= greenDist && redDist <= blueDist && redDist <= blackDist ) {
                color.set( CM_RGB, redColor );
                error = total - redColor;
            }
            else if( greenDist <= redDist && greenDist <= blueDist && greenDist <= blackDist ) {
                color.set( CM_RGB, greenColor );
                error = total - greenColor;
                ;
            }
            else if( blueDist <= redDist && blueDist <= greenDist && blueDist <= blackDist ) {
                color.set( CM_RGB, blueColor );
                error = total - blueColor;
            }
            else if( blackDist <= redDist && blackDist <= greenDist && blackDist <= blueDist ) {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor - redColor - blueColor - greenColor;
            }
            
            error /= 8.0;
            
            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error );
            }
            
            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error );
            }
            
            if( y < ( height - 1 ) ) {
                if( ( x - 1 ) >= 0 ) {
                    auto posLeft = ivec2( x - 1, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error );
                }
                
                auto posCen = ivec2( x, y + 1 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error );
                
                if( x < ( width - 1 ) ) {
                    auto posRgt = ivec2( x + 1, y + 1 );
                    auto pxlRgt = output->getPixel( posRgt );
                    output->setPixel( posRgt, pxlRgt + error );
                }
            }
            
            if( y < ( height - 2 ) ) {
                auto posCen = ivec2( x, y + 2 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error );
            }
            
            output->setPixel( pos, color );
        }
    }
    
    return output;
}

//  Burkes (1/32)
//          X   8   4
//  2   4   8   4   2
Surface32fRef Burkes( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );

    int width = input->getWidth();
    int height = input->getHeight();

    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float whiteDist = length( total - white );
            float blackDist = length( total - black );
            
            if( whiteDist <= blackDist ) {
                color.set( CM_RGB, whiteColor );
                error = total - whiteColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }

            error /= 32.0;

            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 8.0f );
            }

            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 4.0f );
            }

            if( y < ( height - 1 ) ) {
                if( ( x - 1 ) >= 0 ) {
                    auto posLeft = ivec2( x - 1, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error * 4.0f );
                }

                if( ( x - 2 ) >= 0 ) {
                    auto posLeft = ivec2( x - 2, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error * 2.0f );
                }

                auto posCen = ivec2( x, y + 1 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error * 8.0f );

                if( x < ( width - 1 ) ) {
                    auto posRgt = ivec2( x + 1, y + 1 );
                    auto pxlRgt = output->getPixel( posRgt );
                    output->setPixel( posRgt, pxlRgt + error * 4.0f );
                }

                if( x < ( width - 2 ) ) {
                    auto posRgtRgt = ivec2( x + 2, y + 1 );
                    auto pxlRgtRgt = output->getPixel( posRgtRgt );
                    output->setPixel( posRgtRgt, pxlRgtRgt + error * 2.0f );
                }
            }
            output->setPixel( pos, color );
        }
    }

    return output;
}
    
//  Burkes (1/32)
//          X   8   4
//  2   4   8   4   2
Surface32fRef BurkesRGB( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );
    
    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float redDist = length( total - red );
            float greenDist = length( total - green );
            float blueDist = length( total - blue );
            float blackDist = length( total - black );
            
            if( redDist <= greenDist && redDist <= blueDist && redDist <= blackDist ) {
                color.set( CM_RGB, redColor );
                error = total - redColor;
            }
            else if( greenDist <= redDist && greenDist <= blueDist && greenDist <= blackDist ) {
                color.set( CM_RGB, greenColor );
                error = total - greenColor;
                ;
            }
            else if( blueDist <= redDist && blueDist <= greenDist && blueDist <= blackDist ) {
                color.set( CM_RGB, blueColor );
                error = total - blueColor;
            }
            else if( blackDist <= redDist && blackDist <= greenDist && blackDist <= blueDist ) {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor - redColor - blueColor - greenColor;
            }
            
            error /= 32.0;
            
            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 8.0f );
            }
            
            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 4.0f );
            }
            
            if( y < ( height - 1 ) ) {
                if( ( x - 1 ) >= 0 ) {
                    auto posLeft = ivec2( x - 1, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error * 4.0f );
                }
                
                if( ( x - 2 ) >= 0 ) {
                    auto posLeft = ivec2( x - 2, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error * 2.0f );
                }
                
                auto posCen = ivec2( x, y + 1 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error * 8.0f );
                
                if( x < ( width - 1 ) ) {
                    auto posRgt = ivec2( x + 1, y + 1 );
                    auto pxlRgt = output->getPixel( posRgt );
                    output->setPixel( posRgt, pxlRgt + error * 4.0f );
                }
                
                if( x < ( width - 2 ) ) {
                    auto posRgtRgt = ivec2( x + 2, y + 1 );
                    auto pxlRgtRgt = output->getPixel( posRgtRgt );
                    output->setPixel( posRgtRgt, pxlRgtRgt + error * 2.0f );
                }
            }
            output->setPixel( pos, color );
        }
    }
    
    return output;
}

//  Sierra (1/32)
//          X   5   3
//  2   4   5   4   2
//      2   3   2
Surface32fRef Sierra( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );

    int width = input->getWidth();
    int height = input->getHeight();

    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float whiteDist = length( total - white );
            float blackDist = length( total - black );
            
            if( whiteDist <= blackDist ) {
                color.set( CM_RGB, whiteColor );
                error = total - whiteColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }

            error /= 32.0;

            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 5.0f );
            }

            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 3.0f );
            }

            for( int i = 1; i < 3; i++ ) {
                float offset = ( i - 1 ) * 2.0f;
                if( y < ( height - i ) ) {
                    if( ( x - 1 ) >= 0 ) {
                        auto posLeft = ivec2( x - 1, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 4.0f - offset ) );
                    }

                    if( ( x - 2 ) >= 0 ) {
                        auto posLeft = ivec2( x - 2, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 2.0f - offset ) );
                    }

                    auto posCen = ivec2( x, y + i );
                    auto pxlCen = output->getPixel( posCen );
                    output->setPixel( posCen, pxlCen + error * ( 5.0f - offset ) );

                    if( x < ( width - 1 ) ) {
                        auto posRgt = ivec2( x + 1, y + i );
                        auto pxlRgt = output->getPixel( posRgt );
                        output->setPixel( posRgt, pxlRgt + error * ( 4.0f - offset ) );
                    }

                    if( x < ( width - 2 ) ) {
                        auto posRgtRgt = ivec2( x + 2, y + i );
                        auto pxlRgtRgt = output->getPixel( posRgtRgt );
                        output->setPixel( posRgtRgt, pxlRgtRgt + error * ( 2.0f - offset ) );
                    }
                }
            }
            output->setPixel( pos, color );
        }
    }

    return output;
}
    
//  Sierra (1/32)
//          X   5   3
//  2   4   5   4   2
//      2   3   2
Surface32fRef SierraRGB( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );
    
    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float redDist = length( total - red );
            float greenDist = length( total - green );
            float blueDist = length( total - blue );
            float blackDist = length( total - black );
            
            if( redDist <= greenDist && redDist <= blueDist && redDist <= blackDist ) {
                color.set( CM_RGB, redColor );
                error = total - redColor;
            }
            else if( greenDist <= redDist && greenDist <= blueDist && greenDist <= blackDist ) {
                color.set( CM_RGB, greenColor );
                error = total - greenColor;
                ;
            }
            else if( blueDist <= redDist && blueDist <= greenDist && blueDist <= blackDist ) {
                color.set( CM_RGB, blueColor );
                error = total - blueColor;
            }
            else if( blackDist <= redDist && blackDist <= greenDist && blackDist <= blueDist ) {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor - redColor - blueColor - greenColor;
            }
            
            error /= 32.0;
            
            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 5.0f );
            }
            
            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 3.0f );
            }
            
            for( int i = 1; i < 3; i++ ) {
                float offset = ( i - 1 ) * 2.0f;
                if( y < ( height - i ) ) {
                    if( ( x - 1 ) >= 0 ) {
                        auto posLeft = ivec2( x - 1, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 4.0f - offset ) );
                    }
                    
                    if( ( x - 2 ) >= 0 ) {
                        auto posLeft = ivec2( x - 2, y + i );
                        auto pxlLeft = output->getPixel( posLeft );
                        output->setPixel( posLeft, pxlLeft + error * ( 2.0f - offset ) );
                    }
                    
                    auto posCen = ivec2( x, y + i );
                    auto pxlCen = output->getPixel( posCen );
                    output->setPixel( posCen, pxlCen + error * ( 5.0f - offset ) );
                    
                    if( x < ( width - 1 ) ) {
                        auto posRgt = ivec2( x + 1, y + i );
                        auto pxlRgt = output->getPixel( posRgt );
                        output->setPixel( posRgt, pxlRgt + error * ( 4.0f - offset ) );
                    }
                    
                    if( x < ( width - 2 ) ) {
                        auto posRgtRgt = ivec2( x + 2, y + i );
                        auto pxlRgtRgt = output->getPixel( posRgtRgt );
                        output->setPixel( posRgtRgt, pxlRgtRgt + error * ( 2.0f - offset ) );
                    }
                }
            }
            output->setPixel( pos, color );
        }
    }
    
    return output;
}

//  TwoRowSierra (1/16)
//          X   4   3
//  1   2   3   2   1
Surface32fRef TwoRowSierra( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );

    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float whiteDist = length( total - white );
            float blackDist = length( total - black );
            
            if( whiteDist <= blackDist ) {
                color.set( CM_RGB, whiteColor );
                error = total - whiteColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }

            error /= 16.0;

            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 4.0f );
            }

            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 3.0f );
            }

            //  1   2   3   2   1

            if( y < ( height - 1 ) ) {
                if( ( x - 1 ) >= 0 ) {
                    auto posLeft = ivec2( x - 1, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error * 2.0f );
                }

                if( ( x - 2 ) >= 0 ) {
                    auto posLeft = ivec2( x - 2, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error * 1.0f );
                }

                auto posCen = ivec2( x, y + 1 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error * 3.0f );

                if( x < ( width - 1 ) ) {
                    auto posRgt = ivec2( x + 1, y + 1 );
                    auto pxlRgt = output->getPixel( posRgt );
                    output->setPixel( posRgt, pxlRgt + error * 2.0f );
                }

                if( x < ( width - 2 ) ) {
                    auto posRgtRgt = ivec2( x + 2, y + 1 );
                    auto pxlRgtRgt = output->getPixel( posRgtRgt );
                    output->setPixel( posRgtRgt, pxlRgtRgt + error * 1.0f );
                }
            }
            output->setPixel( pos, color );
        }
    }

    return output;
}

//  TwoRowSierra (1/16)
//          X   4   3
//  1   2   3   2   1
Surface32fRef TwoRowSierraRGB( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );
    
    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float redDist = length( total - red );
            float greenDist = length( total - green );
            float blueDist = length( total - blue );
            float blackDist = length( total - black );
            
            if( redDist <= greenDist && redDist <= blueDist && redDist <= blackDist ) {
                color.set( CM_RGB, redColor );
                error = total - redColor;
            }
            else if( greenDist <= redDist && greenDist <= blueDist && greenDist <= blackDist ) {
                color.set( CM_RGB, greenColor );
                error = total - greenColor;
                ;
            }
            else if( blueDist <= redDist && blueDist <= greenDist && blueDist <= blackDist ) {
                color.set( CM_RGB, blueColor );
                error = total - blueColor;
            }
            else if( blackDist <= redDist && blackDist <= greenDist && blackDist <= blueDist ) {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor - redColor - blueColor - greenColor;
            }
            
            error /= 16.0;
            
            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 4.0f );
            }
            
            if( x < ( width - 2 ) ) {
                auto pos = ivec2( x + 2, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 3.0f );
            }
            
            //  1   2   3   2   1
            
            if( y < ( height - 1 ) ) {
                if( ( x - 1 ) >= 0 ) {
                    auto posLeft = ivec2( x - 1, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error * 2.0f );
                }
                
                if( ( x - 2 ) >= 0 ) {
                    auto posLeft = ivec2( x - 2, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error * 1.0f );
                }
                
                auto posCen = ivec2( x, y + 1 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error * 3.0f );
                
                if( x < ( width - 1 ) ) {
                    auto posRgt = ivec2( x + 1, y + 1 );
                    auto pxlRgt = output->getPixel( posRgt );
                    output->setPixel( posRgt, pxlRgt + error * 2.0f );
                }
                
                if( x < ( width - 2 ) ) {
                    auto posRgtRgt = ivec2( x + 2, y + 1 );
                    auto pxlRgtRgt = output->getPixel( posRgtRgt );
                    output->setPixel( posRgtRgt, pxlRgtRgt + error * 1.0f );
                }
            }
            output->setPixel( pos, color );
        }
    }
    
    return output;
}

//  SierraLite (1/4)
//      X   2
//  1   1
Surface32fRef SierraLite( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );
    
    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float whiteDist = length( total - white );
            float blackDist = length( total - black );
            
            if( whiteDist <= blackDist ) {
                color.set( CM_RGB, whiteColor );
                error = total - whiteColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }
            
            error /= 4.0;
            
            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 2.0f );
            }
            
            if( y < ( height - 1 ) ) {
                if( ( x - 1 ) >= 0 ) {
                    auto posLeft = ivec2( x - 1, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error );
                }
                
                auto posCen = ivec2( x, y + 1 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error );
            }
            output->setPixel( pos, color );
        }
    }
    
    return output;
}
    
//  SierraLite (1/4)
//      X   2
//  1   1
Surface32fRef SierraLiteRGB( Surface32fRef input )
{
    auto output = Surface32f::create( input->getWidth(), input->getHeight(), input->hasAlpha() );

    int width = input->getWidth();
    int height = input->getHeight();
    
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            ivec2 pos( x, y );
            ColorA color = input->getPixel( pos );
            ColorA error = output->getPixel( pos );
            const ColorA total = error + color;
            
            float redDist = length( total - red );
            float greenDist = length( total - green );
            float blueDist = length( total - blue );
            float blackDist = length( total - black );
            
            if( redDist <= greenDist && redDist <= blueDist && redDist <= blackDist ) {
                color.set( CM_RGB, redColor );
                error = total - redColor;
            }
            else if( greenDist <= redDist && greenDist <= blueDist && greenDist <= blackDist ) {
                color.set( CM_RGB, greenColor );
                error = total - greenColor;
                ;
            }
            else if( blueDist <= redDist && blueDist <= greenDist && blueDist <= blackDist ) {
                color.set( CM_RGB, blueColor );
                error = total - blueColor;
            }
            else if( blackDist <= redDist && blackDist <= greenDist && blackDist <= blueDist ) {
                color.set( CM_RGB, blackColor );
                error = total - blackColor;
            }
            else {
                color.set( CM_RGB, blackColor );
                error = total - blackColor - redColor - blueColor - greenColor;
            }

            error /= 4.0;

            if( x < ( width - 1 ) ) {
                auto pos = ivec2( x + 1, y );
                auto pxl = output->getPixel( pos );
                output->setPixel( pos, pxl + error * 2.0f );
            }

            if( y < ( height - 1 ) ) {
                if( ( x - 1 ) >= 0 ) {
                    auto posLeft = ivec2( x - 1, y + 1 );
                    auto pxlLeft = output->getPixel( posLeft );
                    output->setPixel( posLeft, pxlLeft + error );
                }

                auto posCen = ivec2( x, y + 1 );
                auto pxlCen = output->getPixel( posCen );
                output->setPixel( posCen, pxlCen + error );
            }
            output->setPixel( pos, color );
        }
    }

    return output;
}
    
}
}
