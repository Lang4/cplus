/**********************************************************************************
 * Author jijinlong Date 2014/3/30
 * email:jjl_2009_hi@163.com
 */
#pragma once
#include "XmlCode.h"
#include "Script.h"

class XmlScript:public Script{
public:
	void readFromFile(const std::string & fileName)
	{
		clearCodes();
		tinyxml2::XMLDocument doc;
        if (doc.LoadFile(fileName.c_str())){
            printf("file cant found %s\n", fileName.c_str());
            return;
        } 
        tinyxml2::XMLElement * root = doc.RootElement();
        if (!root) {
            printf("root element not found\n");
            return ;
        }
		while (root)
		{
			if (!strcmp(root->Value(),"code"))
			{
				XmlCode * code = (XmlCode*)generate(root->Attribute("lib"));
				if (!code) return;
				CodeNode * node = code->traceNode(root);
				std::string codeName = root->Attribute("name") ? root->Attribute("name"):"";
				code->code = node;
				codes[codeName] = code;
			}
			root = root->NextSiblingElement("code");
		}
	}
	virtual ~XmlScript(){}
};


#define XML_SCRIPT(CLASS)\
class CLASS:public XmlCode{\
public:\
    CLASS(){\
		init();\
	};\
	XmlCode * clone()\
	{\
		XmlCode * code = new CLASS();\
		*code = *this;\
		return code;\
	}\
	void init();\
};\
AutoRunXML<CLASS> auto##CLASS(#CLASS);\
void CLASS::init()

#define theXml Script::getMe<XmlScript>()
template<typename CLASS>
class AutoRunXML{
public:
	AutoRunXML(const char *name)
	{
		theXml.addLib(name,new CLASS());
	}
};

