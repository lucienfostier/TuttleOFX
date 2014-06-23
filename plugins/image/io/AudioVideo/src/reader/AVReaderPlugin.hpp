#ifndef _TUTTLE_PLUGIN_AV_READER_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_AV_READER_PLUGIN_HPP_

#include <AvTranscoder/InputFile.hpp>
#include <AvTranscoder/InputStreamVideo.hpp>
#include <AvTranscoder/DatasStructures/Image.hpp>
#include <AvTranscoder/ColorTransform.hpp>

#include <tuttle/plugin/context/ReaderPlugin.hpp>

#include <boost/scoped_ptr.hpp>

#include <string>

namespace tuttle {
namespace plugin {
namespace av {
namespace reader {

struct AVReaderParams
{
	std::string _filepath;      ///< Ffmpeg filepath
};

/**
 * @brief AudioVideo plugin
 */
class AVReaderPlugin : public ReaderPlugin
{
public:
	AVReaderPlugin( OfxImageEffectHandle handle );

public:
	void ensureVideoIsOpen( const std::string& path );
	
	AVReaderParams getProcessParams() const;

	void updateVisibleTools();
	void changedParam( const OFX::InstanceChangedArgs& args, const std::string& paramName );
	bool getRegionOfDefinition( const OFX::RegionOfDefinitionArguments& args, OfxRectD& rod );
	void getClipPreferences( OFX::ClipPreferencesSetter& clipPreferences );
	bool getTimeDomain( OfxRangeD& range );

	void beginSequenceRender( const OFX::BeginSequenceRenderArguments& args );
	void render( const OFX::RenderArguments& args );

public:
	// do not need to delete these, the ImageEffect is managing them for us
	OFX::Clip*         _clipDst;           ///< Destination image clip

	OFX::IntParam* _paramVideoStreamIndex; ///< video stream index
	OFX::BooleanParam* _paramUseCustomSAR; ///< Keep sample aspect ratio
	OFX::DoubleParam*  _paramCustomSAR;    ///< Custom SAR to use
	
	boost::scoped_ptr<avtranscoder::InputFile> _inputFile;
	boost::scoped_ptr<avtranscoder::InputStreamVideo> _inputStreamVideo;
	boost::scoped_ptr<avtranscoder::Image> _sourceImage;
	boost::scoped_ptr<avtranscoder::Image> _imageToDecode;
	
	avtranscoder::ColorTransform _colorTransform;
	
	std::string _lastInputFilePath;
	size_t _idVideoStream;
	size_t _lastFrame;
};

}
}
}
}

#endif
