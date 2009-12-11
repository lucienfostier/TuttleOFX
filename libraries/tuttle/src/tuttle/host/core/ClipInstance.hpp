/*
 * Software License :
 *
 * Copyright (c) 2007, The Open Effects Association Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * Neither the name The Open Effects Association Ltd, nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TUTTLE_CLIP_INSTANCE_H
#define TUTTLE_CLIP_INSTANCE_H

#include "EffectInstance.hpp"
#include <tuttle/host/ofx/OfxhImageEffect.hpp>
#include <tuttle/host/ofx/OfxhImage.hpp>
#include <boost/cstdint.hpp>

#define SOFXCLIPLENGTH 1

namespace tuttle {
namespace host {
namespace core {

// foward
class ClipImgInstance;

/// make an image up

class Image : public tuttle::host::ofx::imageEffect::Image
{
protected:
	size_t _ncomp; // number of components
	boost::uint8_t* _data; // where we are keeping our image data

public:
	explicit Image( ClipImgInstance& clip, const OfxRectD& bounds, OfxTime t );
	virtual ~Image();

	boost::uint8_t* getPixelData()
	{
		return _data;
	}

	boost::uint8_t* pixel( int x, int y ) const;
	static void     copy( Image* dst, Image* src, const OfxPointI& dstCorner,
	                      const OfxPointI& srcCorner, const OfxPointI& count );
	template < class VIEW_T >
	static VIEW_T gilViewFromImage( Image* img );

private:
	template < class S_VIEW >
	static void copy( Image* dst, S_VIEW& src, const OfxPointI& dstCorner,
	                  const OfxPointI& srcCorner, const OfxPointI& count );
	template < class D_VIEW, class S_VIEW >
	static void copy( D_VIEW& dst, S_VIEW& src, const OfxPointI& dstCorner,
	                  const OfxPointI& srcCorner, const OfxPointI& count );
};

class ClipImgInstance : public tuttle::host::ofx::attribute::ClipImageInstance
{
protected:
	const EffectInstance& _effect;
	std::string _name;
	Image* _inputImage; ///< input clip image @todo tuttle: variable used in rendering process, need to be moved to ProcessNode ?
	Image* _outputImage; ///< output clip image @todo tuttle: variable used in rendering process, need to be moved to ProcessNode ?
	OfxPointD _frameRange; ///< get frame range

public:
	ClipImgInstance( EffectInstance& effect, const tuttle::host::ofx::attribute::ClipImageDescriptor& desc );

	~ClipImgInstance();

	ClipImgInstance* clone() const { return new ClipImgInstance(*this); };

	Image* getInputImage()
	{
		return _inputImage;
	}

	Image* getOutputImage()
	{
		return _outputImage;
	}

	/// Get the Raw Unmapped Pixel Depth from the host
	///
	/// \returns
	///    - kOfxBitDepthNone (implying a clip is unconnected image)
	///    - kOfxBitDepthByte
	///    - kOfxBitDepthShort
	///    - kOfxBitDepthFloat
	const std::string& getUnmappedBitDepth() const;

	/// Get the Raw Unmapped Components from the host
	///
	/// \returns
	///     - kOfxImageComponentNone (implying a clip is unconnected, not valid for an image)
	///     - kOfxImageComponentRGBA
	///     - kOfxImageComponentAlpha
	const std::string& getUnmappedComponents() const;

	// PreMultiplication -
	//
	//  kOfxImageOpaque - the image is opaque and so has no premultiplication state
	//  kOfxImagePreMultiplied - the image is premultiplied by it's alpha
	//  kOfxImageUnPreMultiplied - the image is unpremultiplied
	const std::string& getPremult() const;

	// Frame Rate -
	//
	//  The frame rate of a clip or instance's project.
	double getFrameRate() const;

	// Frame Range (startFrame, endFrame) -
	//
	//  The frame range over which a clip has images.
	void getFrameRange( double& startFrame, double& endFrame ) const;

	/// Field Order - Which spatial field occurs temporally first in a frame.
	/// \returns
	///  - kOfxImageFieldNone - the clip material is unfielded
	///  - kOfxImageFieldLower - the clip material is fielded, with image rows 0,2,4.... occuring first in a frame
	///  - kOfxImageFieldUpper - the clip material is fielded, with image rows line 1,3,5.... occuring first in a frame
	const std::string& getFieldOrder() const;

	// Connected -
	//
	//  Says whether the clip is actually connected at the moment.
	bool getConnected() const;

	// Unmapped Frame Rate -
	//
	//  The unmaped frame range over which an output clip has images.
	double getUnmappedFrameRate() const;

	// Unmapped Frame Range -
	//
	//  The unmaped frame range over which an output clip has images.
	void getUnmappedFrameRange( double& unmappedStartFrame, double& unmappedEndFrame ) const;

	// Continuous Samples -
	//
	//  0 if the images can only be sampled at discreet times (eg: the clip is a sequence of frames),
	//  1 if the images can only be sampled continuously (eg: the clip is infact an animating roto spline and can be rendered anywhen).
	bool getContinuousSamples() const;

	/// override this to fill in the image at the given time.
	/// The bounds of the image on the image plane should be
	/// 'appropriate', typically the value returned in getRegionsOfInterest
	/// on the effect instance. Outside a render call, the optionalBounds should
	/// be 'appropriate' for the.
	/// If bounds is not null, fetch the indicated section of the canonical image plane.
	tuttle::host::ofx::imageEffect::Image* getImage( OfxTime time, OfxRectD* optionalBounds = NULL );

	/// override this to return the rod on the clip
	OfxRectD getRegionOfDefinition( OfxTime time ) const;
};

}
}
}

#endif
