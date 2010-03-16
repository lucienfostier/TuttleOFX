#ifndef _TUTTLE_PLUGIN_FFMPEG_READER_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_FFMPEG_READER_PLUGIN_HPP_

#include <tuttle/common/utils/global.hpp>
#include <ofxsImageEffect.h>
#include <string>
#include <boost/gil/gil_all.hpp>
#include <boost/scoped_ptr.hpp>
#include <ffmpeg/VideoFFmpegReader.hpp>

namespace tuttle {
namespace plugin {
namespace ffmpeg {
namespace reader {

struct FFMpegReaderParams
{
	std::string _filepath;      ///< Ffmpeg filepath
};

/**
 * @brief FFMpeg plugin
 */
class FFMpegReaderPlugin : public OFX::ImageEffect
{
public:
    FFMpegReaderPlugin( OfxImageEffectHandle handle );
    OFX::Clip *getSrcClip( ) const;
    OFX::Clip *getDstClip( ) const;
	VideoFFmpegReader & getReader();

public:
    virtual void render( const OFX::RenderArguments &args );
    void changedParam( const OFX::InstanceChangedArgs &args, const std::string &paramName );
	bool getTimeDomain( OfxRangeD& range );
	FFMpegReaderParams getParams() const;
	void beginSequenceRender( const OFX::BeginSequenceRenderArguments& args );
	void endSequenceRender( const OFX::EndSequenceRenderArguments& args );
	void getClipPreferences( OFX::ClipPreferencesSetter& clipPreferences );

protected:
    // do not need to delete these, the ImageEffect is managing them for us
    OFX::Clip           *_srcClip;       ///< Source image clip
    OFX::Clip           *_dstClip;       ///< Destination image clip
	boost::scoped_ptr<std::string> _openedSource;  ///< Opened video file source
	OFX::StringParam    *_filepath;      ///< Ffmpeg filepath
	VideoFFmpegReader	_reader;
};

}
}
}
}

#endif