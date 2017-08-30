#pragma once

#include "cinder/Surface.h"
#include "cinder/Color.h"

namespace reza {
namespace dither {
ci::Surface32fRef linear( ci::Surface32fRef input );
ci::Surface32fRef linearRGB( ci::Surface32fRef input );
ci::Surface32fRef FloydSteinberg( ci::Surface32fRef input );
ci::Surface32fRef FloydSteinbergRGB( ci::Surface32fRef input );
ci::Surface32fRef JarvisJudiceNinke( ci::Surface32fRef input );
ci::Surface32fRef JarvisJudiceNinkeRGB( ci::Surface32fRef input );
ci::Surface32fRef Stucki( ci::Surface32fRef input );
ci::Surface32fRef StuckiRGB( ci::Surface32fRef input );
ci::Surface32fRef Atkinson( ci::Surface32fRef input );
ci::Surface32fRef AtkinsonRGB( ci::Surface32fRef input );
ci::Surface32fRef Burkes( ci::Surface32fRef input );
ci::Surface32fRef BurkesRGB( ci::Surface32fRef input );
ci::Surface32fRef Sierra( ci::Surface32fRef input );
ci::Surface32fRef SierraRGB( ci::Surface32fRef input );
ci::Surface32fRef TwoRowSierra( ci::Surface32fRef input );
ci::Surface32fRef TwoRowSierraRGB( ci::Surface32fRef input );
ci::Surface32fRef SierraLite( ci::Surface32fRef input );
ci::Surface32fRef SierraLiteRGB( ci::Surface32fRef input );
}
}
