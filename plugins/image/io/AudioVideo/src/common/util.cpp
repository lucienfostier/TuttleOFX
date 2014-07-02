#include "util.hpp"

#include <writer/AVWriterDefinitions.hpp>

#include <AvTranscoder/Option.hpp>

#include <boost/foreach.hpp>

#include <iostream>

namespace tuttle {
namespace plugin {
namespace av {
namespace common {

void addOptionsToGroup( OFX::ImageEffectDescriptor& desc, OFX::GroupParamDescriptor* group, avtranscoder::OptionLoader::OptionArray& optionsArray, const std::string& prefix )
{
	OFX::ParamDescriptor* param = NULL;
	BOOST_FOREACH( avtranscoder::Option& option, optionsArray )
	{
		std::string name = prefix;
		name += option.getName();
		
		switch( option.getType() )
		{
			case avtranscoder::TypeBool:
			{
				OFX::BooleanParamDescriptor* boolParam = desc.defineBooleanParam( name );
				boolParam->setDefault( option.getDefaultValueBool() );
				param = boolParam;
				break;
			}
			case avtranscoder::TypeInt:
			{
				OFX::IntParamDescriptor* intParam = desc.defineIntParam( name );
				intParam->setDefault( option.getDefaultValueInt() );
				intParam->setRange( option.getMin(), option.getMax() );
				intParam->setDisplayRange( option.getMin(), option.getMax() );
				param = intParam;
				break;
			}
			case avtranscoder::TypeDouble:
			{
				OFX::DoubleParamDescriptor* doubleParam = desc.defineDoubleParam( name );
				doubleParam->setDefault( option.getDefaultValueDouble() );
				doubleParam->setRange( option.getMin(), option.getMax() );
				doubleParam->setDisplayRange( option.getMin(), option.getMax() );
				param = doubleParam;
				break;
			}
			case avtranscoder::TypeString:
			{
				OFX::StringParamDescriptor* strParam = desc.defineStringParam( name );
				strParam->setDefault( option.getDefaultValueString() );
				param = strParam;
				break;
			}
			case avtranscoder::TypeRatio:
			{
				OFX::Int2DParamDescriptor* ratioParam = desc.defineInt2DParam( name );
				ratioParam->setDefault( option.getDefaultValueRatio().first, option.getDefaultValueRatio().second );
				param = ratioParam;
				break;
			}
			case avtranscoder::TypeChoice:
			{
				OFX::ChoiceParamDescriptor* choiceParam = desc.defineChoiceParam( name );
				choiceParam->setDefault( option.getDefaultChildIndex() );
				BOOST_FOREACH( const avtranscoder::Option& child, option.getChilds() )
				{
					choiceParam->appendOption( child.getName(), child.getHelp() );
				}
				param = choiceParam;
				break;
			}
			case avtranscoder::TypeGroup:
			{
				std::string groupName = prefix;
				groupName += "g_";
				groupName += option.getName();
				OFX::GroupParamDescriptor* groupParam = desc.defineGroupParam( groupName );
				BOOST_FOREACH( const avtranscoder::Option& child, option.getChilds() )
				{
					std::string childName = prefix;
					childName += child.getName();
					
					OFX::BooleanParamDescriptor* param = desc.defineBooleanParam( childName );
					param->setLabel( child.getName() );
					param->setDefault( child.getOffset() );
					param->setHint( child.getHelp() );
					param->setParent( groupParam );
				}
				param = groupParam;
				break;
			}
			default:
				break;
		}
		if( param )
		{
			param->setLabel( option.getName() );
			param->setHint( option.getHelp() );
			param->setParent( group );
		}
	}
}

void addOptionsToGroup( OFX::ImageEffectDescriptor& desc, OFX::GroupParamDescriptor* group,  avtranscoder::OptionLoader::OptionMap& optionsMap, const std::string& prefix )
{
	// iterate on map keys
	BOOST_FOREACH( avtranscoder::OptionLoader::OptionMap::value_type& subGroupOption, optionsMap )
	{
		OFX::ParamDescriptor* param = NULL;
		
		std::string subGroupName = subGroupOption.first;
		std::vector<avtranscoder::Option>& options = subGroupOption.second;
				
		// iterate on options
		BOOST_FOREACH( avtranscoder::Option& option, options )
		{
			std::string name = prefix;
			name += subGroupName;
			name += "_";
			name += option.getName();
			
			switch( option.getType() )
			{
				case avtranscoder::TypeBool:
				{
					OFX::BooleanParamDescriptor* boolParam = desc.defineBooleanParam( name );
					boolParam->setDefault( option.getDefaultValueBool() );
					param = boolParam;
					break;
				}
				case avtranscoder::TypeInt:
				{
					OFX::IntParamDescriptor* intParam = desc.defineIntParam( name );
					intParam->setDefault( option.getDefaultValueInt() );
					intParam->setRange( option.getMin(), option.getMax() );
					intParam->setDisplayRange( option.getMin(), option.getMax() );
					param = intParam;
					break;
				}
				case avtranscoder::TypeDouble:
				{
					OFX::DoubleParamDescriptor* doubleParam = desc.defineDoubleParam( name );
					doubleParam->setDefault( option.getDefaultValueDouble() );
					doubleParam->setRange( option.getMin(), option.getMax() );
					doubleParam->setDisplayRange( option.getMin(), option.getMax() );
					param = doubleParam;
					break;
				}
				case avtranscoder::TypeString:
				{
					OFX::StringParamDescriptor* strParam = desc.defineStringParam( name );
					strParam->setDefault( option.getDefaultValueString() );
					param = strParam;
					break;
				}
				case avtranscoder::TypeRatio:
				{
					OFX::Int2DParamDescriptor* ratioParam = desc.defineInt2DParam( name );
					ratioParam->setDefault( option.getDefaultValueRatio().first, option.getDefaultValueRatio().second );
					param = ratioParam;
					break;
				}
				case avtranscoder::TypeChoice:
				{
					OFX::ChoiceParamDescriptor* choiceParam = desc.defineChoiceParam( name );
					choiceParam->setDefault( option.getDefaultChildIndex() );
					BOOST_FOREACH( const avtranscoder::Option& child, option.getChilds() )
					{
						choiceParam->appendOption( child.getName(), child.getHelp() );
					}
					param = choiceParam;
					break;
				}
				case avtranscoder::TypeGroup:
				{
					std::string groupName = prefix;
					groupName += "g_";
					groupName += subGroupName;
					groupName += "_";
					groupName += option.getName();
					
					OFX::GroupParamDescriptor* groupParam = desc.defineGroupParam( groupName );
					BOOST_FOREACH( const avtranscoder::Option& child, option.getChilds() )
					{
						std::string childName = prefix;
						childName += "flags_";
						childName += subGroupName;
						childName += "_";
						childName += child.getName();
						
						OFX::BooleanParamDescriptor* param = desc.defineBooleanParam( childName );
						param->setLabel( child.getName() );
						param->setDefault( child.getOffset() );
						param->setHint( child.getHelp() );
						param->setParent( groupParam );
					}
					param = groupParam;
					break;
				}
				default:
					break;
			}
			if( param )
			{
				param->setLabel( option.getName() );
				param->setHint( option.getHelp() );
				param->setParent( group );
			}
		}
	}
}

std::string getOptionNameWithoutPrefix( const std::string& optionName, const std::string& codecName )
{
	std::string nameWithoutPrefix( optionName );
	
	size_t prefixPosition;
	if( ( prefixPosition = nameWithoutPrefix.find( writer::kPrefixFormat ) ) != std::string::npos )
		nameWithoutPrefix.erase( prefixPosition, writer::kPrefixFormat.size() );
	else if( ( prefixPosition = nameWithoutPrefix.find( writer::kPrefixVideo ) ) != std::string::npos )
		nameWithoutPrefix.erase( prefixPosition, writer::kPrefixVideo.size() );
	else if( ( prefixPosition = nameWithoutPrefix.find( writer::kPrefixAudio ) ) != std::string::npos )
		nameWithoutPrefix.erase( prefixPosition, writer::kPrefixAudio.size() );
	
	// groups
	const std::string prefixGroup( "g_" );
	if( ( prefixPosition = nameWithoutPrefix.find( prefixGroup ) ) != std::string::npos )
	{
		nameWithoutPrefix.erase( prefixPosition, prefixGroup.size() );
	}
	
	// childs of groups
	const std::string prefixChild( "flags_" );
	if( ( prefixPosition = nameWithoutPrefix.find( prefixChild ) ) != std::string::npos )
	{
		nameWithoutPrefix.erase( prefixPosition, prefixChild.size() );
	}
	
	// codec name
	if( ( prefixPosition = nameWithoutPrefix.find( codecName ) ) != std::string::npos )
	{
		// codecName.size() + 1: also remove the "_"
		nameWithoutPrefix.erase( prefixPosition, codecName.size() + 1 );
	}
	
	return nameWithoutPrefix;
}

}
}
}
}
