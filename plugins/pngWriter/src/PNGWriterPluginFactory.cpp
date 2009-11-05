/**
 * @author Eloi Du Bois.
 * @date   2009/10/11
 *
 */

#include "PNGWriterPlugin.hpp"
#include "tuttle/plugin/ImageGilProcessor.hpp"
#include "tuttle/plugin/Progress.hpp"
#include "tuttle/plugin/PluginException.hpp"

#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ofxsImageEffect.h>
#include <ofxsMultiThread.h>
#include <boost/gil/gil_all.hpp>
#include <boost/scoped_ptr.hpp>

namespace tuttle {

static const bool kSupportTiles     = false;

mDeclarePluginFactory(PNGWriterPluginFactory, {}, {});

/**
 * @brief Function called to describe the plugin main features.
 * @param[in, out]   desc     Effect descriptor
 */
using namespace OFX;
void
PNGWriterPluginFactory::describe(OFX::ImageEffectDescriptor &desc)
{
    // basic labels
    desc.setLabels("PNGWriterHd3d", "PNGWriterHd3d",
                   "PNG File writer Hd3d");
    desc.setPluginGrouping("hd3d");

    // add the supported contexts, only filter at the moment
//    desc.addSupportedContext(eContextGenerator);
    desc.addSupportedContext(eContextGeneral);

    // add supported pixel depths
    desc.addSupportedBitDepth(eBitDepthUByte);
    desc.addSupportedBitDepth(eBitDepthUShort);
    desc.addSupportedBitDepth(eBitDepthFloat);

    // set a few flags
    desc.setSingleInstance(false);
    desc.setHostFrameThreading(false);
    desc.setSupportsMultiResolution(false);
    desc.setSupportsTiles(kSupportTiles);
    desc.setTemporalClipAccess(false);
    desc.setRenderTwiceAlways(false);
    desc.setSupportsMultipleClipPARs(false);
}

/**
 * @brief Function called to describe the plugin controls and features.
 * @param[in, out]   desc       Effect descriptor
 * @param[in]        context    Application context
 */
void
PNGWriterPluginFactory::describeInContext(OFX::ImageEffectDescriptor &desc,
                                          OFX::ContextEnum context)
{
    ClipDescriptor *srcClip = desc.defineClip( kOfxImageEffectSimpleSourceClipName );
    srcClip->addSupportedComponent( ePixelComponentRGBA );
    srcClip->addSupportedComponent( ePixelComponentAlpha );
    srcClip->setSupportsTiles( kSupportTiles );

    ClipDescriptor *dstClip = desc.defineClip( "Output" );
    dstClip->addSupportedComponent( ePixelComponentRGBA );
    dstClip->addSupportedComponent( ePixelComponentAlpha );
    dstClip->setSupportsTiles( kSupportTiles );

    // Controls
    StringParamDescriptor *filename = desc.defineStringParam( kInputFilename );
    filename->setScriptName( "filename" );
    filename->setStringType( eStringTypeFilePath );
    filename->setCacheInvalidation( eCacheInvalidateValueAll );

    PushButtonParamDescriptor *renderButton = desc.definePushButtonParam( kRender );
    renderButton->setScriptName( "renderButton" );
}

/**
 * @brief Function called to create a plugin effect instance
 * @param[in] handle  effect handle
 * @param[in] context    Application context
 * @return  plugin instance
 */
OFX::ImageEffect*
PNGWriterPluginFactory::createInstance(OfxImageEffectHandle handle,
                                            OFX::ContextEnum context)
{
    return new PNGWriterPlugin(handle);
}

}

namespace OFX 
{
    namespace Plugin 
    {
        void getPluginIDs(OFX::PluginFactoryArray &ids)
        {
            static tuttle::PNGWriterPluginFactory p("fr.hd3d.tuttle.pngWriter", 1, 0);
            ids.push_back(&p);
        }
    }
}
