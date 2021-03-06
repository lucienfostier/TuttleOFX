%include <tuttle/host/global.i>
%include <std_list.i>
%include <std_vector.i>
%include <std_string.i>

%{
#include <tuttle/host/NodeListArg.hpp>
#include <tuttle/host/INode.hpp>
%}

namespace std {
%template(StringVector) vector<string>;
%template(StringList) list<string>;
%template(NodePtrVector) vector<tuttle::host::INode*>;
%template(NodePtrList) list<tuttle::host::INode*>;
}

%implicitconv tuttle::host::NodeListArg;

%include <tuttle/host/NodeListArg.hpp>

