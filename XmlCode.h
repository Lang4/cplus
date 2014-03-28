/**********************************************************************************
 * Author jijinlong Date 2014/3/30
 * email:jjl_2009_hi@163.com
 */
#pragma once
#include "tinyxml2.h"
#include "Lang.h"
#include "Code.h"
class XmlCode:public Code{
public:
	XmlCode()
	{
		
	}
	virtual ~XmlCode(){}
    /**
     * 从配置文件中读取node
     */
    CodeNode * readFromFile(const char *fileName)
    {
        tinyxml2::XMLDocument doc;
		innerVars.clear();
		conditions.clear();
        if (doc.LoadFile(fileName)){
            printf("file cant found %s\n", fileName);
            return NULL;
        } 
        tinyxml2::XMLElement * root = doc.RootElement();
        if (!root) {
            printf("root element not found\n");
            return NULL;
        }
        if (!strcmp(root->Value(),"code"))
        {
            return traceNode(root);
        }
        printf("code not in xml script\n");
        return NULL;
    }
    CodeNode * traceNode(tinyxml2::XMLElement *root)
    {
        CodeNode *codeRoot = NULL;
        if (NULL == root) return NULL;
        tinyxml2::XMLNode *now = root;
        std::vector<tinyxml2::XMLNode*> parents;
        std::vector<CodeNode* > rootNodes;
		bool over = false;
        while(now && !over)
        {
            CodeNode* nowCode = parseNode(now,codeRoot);
            if (!nowCode){
#if 1 
                printf("%s\n", "cant find logic node");
#endif
                break;
            } 
            tinyxml2::XMLNode * child = now->FirstChildElement();
            if (!child)
            {
				if (now == root) {
					over = true;	
					break;
				}
                now = now->NextSiblingElement();
                while (!now)
                {
                    if (parents.empty()) {
						over = true;
                        break;
                    }
                    tinyxml2::XMLNode * parent = parents.back();
                    codeRoot = rootNodes.back();
                    parents.pop_back();
                    rootNodes.pop_back();
                    now = parent->NextSiblingElement();
                }
				if (now == root) {
					over = true;	
					break;
				}
				if (now && now->Value() && !strcmp(now->Value(),"code"))
				{
					over = true;
					break;
				}
            }
            else
            {
                parents.push_back(now);
                rootNodes.push_back(codeRoot);
                now = child;
                codeRoot = nowCode;
            }
        }
        //printf("create root %x\n", codeRoot);
        return codeRoot;
    }
    CodeNode* parseNode(tinyxml2::XMLNode *now,CodeNode * & codeRoot)
    {
        if (NULL == now) return NULL;
        tinyxml2::XMLElement *element = now->ToElement();
        tinyxml2::XMLNode * text = now->FirstChild();
        if (text)
        {
            tinyxml2::XMLText * content = text->ToText();
            if (content)
            {
                printf("%s\n", content->Value());
            }
        }
        if (element && element->Value() )
        {
            #if _DEBUG 
            printf("parse element %s\n", element->Value());
            #endif
            if (!strcmp(element->Value(),"code") )
            {
                if (!codeRoot)
                {
                    codeRoot = new CodeNode();
					codeRoot->nodeName = element->Value();
                    return codeRoot;
                }
                return NULL;
            }
            else if (!strcmp(element->Value(),"and") && codeRoot)
            {
                AndNode *andNode = new AndNode();
				andNode->nodeName = element->Value();
                codeRoot->addChild(andNode);
                return andNode;
            }
            else if (!strcmp(element->Value(),"block") && codeRoot)
            {
                CodeNode *codeNode = new CodeNode();
				codeNode->nodeName = element->Value();
                codeRoot->addChild(codeNode);
                return codeNode;
            }
            else if (!strcmp(element->Value(),"or") && codeRoot)
            {
                OrNode *orNode = new OrNode();
				orNode->nodeName = element->Value();
                codeRoot->addChild(orNode);
                return orNode;
            }
            else if ((!strcmp(element->Value(),"when") || !strcmp(element->Value(),"case") )&& codeRoot)
            {
                WhenNode *whenNode = new WhenNode();
				whenNode->nodeName = element->Value();
                codeRoot->addChild(whenNode);
                std::string value = element->Attribute("state");
                if (value == "NO") whenNode->state = CodeNode::NO;
                else whenNode->state = CodeNode::YES;

                // 初始化condition 变量
                whenNode->condition = findCondition(element->Attribute("condition")?element->Attribute("condition"):"");
                return whenNode;
            }
            else if (!strcmp(element->Value(),"while") && codeRoot)
            {
                WhileNode *whileNode = new WhileNode();
                codeRoot->addChild(whileNode);
                std::string value = element->Attribute("state")?element->Attribute("state"):"";
                if (value == "NO") whileNode->state = CodeNode::NO;
                else whileNode->state = CodeNode::YES;

                // 初始化condition 变量
                whileNode->condition = findCondition(element->Attribute("condition")?element->Attribute("condition"):"");
                return whileNode;
            }
			else if (!strcmp(element->Value(),"foreach") && codeRoot)
			{
				ForEachNode *forNode = new ForEachNode();
				codeRoot->addChild(forNode);
                std::string value = element->Attribute("state")?element->Attribute("state"):"";
                if (value == "NO") forNode->state = CodeNode::NO;
                else forNode->state = CodeNode::YES;

                // 初始化condition 变量
				forNode->condition = findCondition(element->Attribute("condition")?element->Attribute("condition"):"");
				
				forNode->targets = element->Attribute("targets")?element->Attribute("targets"):"";
				forNode->objectName = element->Attribute("object")?element->Attribute("object"):"";

				return forNode;
			}
            else if (!strcmp(element->Value(),"for") && codeRoot)
            {
                ForNode *forNode = new ForNode();
                codeRoot->addChild(forNode);
                forNode->start = element->Attribute("start")?element->Attribute("start"):"";
                forNode->end = element->Attribute("end")?element->Attribute("end"):"";
                forNode->step = element->Attribute("step")?element->Attribute("step"):"1";
                return forNode;
            }
            else if (!strcmp(element->Value(),"condition") && codeRoot)
            {
                ConditionNode *cNode = new ConditionNode();
                //codeRoot->addChild(cNode);
                // 存储condition 
                std::string name = element->Attribute("name");
                addCondition(name,cNode);
                return cNode;
            }
            else if (!strcmp(element->Value(),"switch") && codeRoot)
            {
                SwitchNode *sNode = new SwitchNode();
                codeRoot->addChild(sNode);
                return sNode;
            }
            else if (!strcmp(element->Value(),"not") && codeRoot)
            {
                NotNode *nNode = new NotNode();
                codeRoot->addChild(nNode);
                std::string actionName = element->Attribute("action");
                parseLogic(actionName,nNode,element); 
                return nNode;
            }
            else if (!strcmp(element->Value(),"var") && codeRoot)
            {
                VarNode *varNode = new VarNode();
				varNode->nodeName = element->Value();
                if (parseVar(varNode,element))
                {
                    codeRoot->addChild(varNode);
                    return varNode;
                }
                else
                {
                    printf("parse var error!!\n");
                    return varNode;
                }
            }
			else if (!strcmp(element->Value(),"calc") && codeRoot)
			{
				CalcNode *cNode = new CalcNode();
                codeRoot->addChild(cNode);
				parseAttribute(cNode,element);
                return cNode;
			}
			else if (!strcmp(element->Value(),"print") && codeRoot)
			{
				PrintNode *pNode = new PrintNode();
				codeRoot->addChild(pNode);
				parseAttribute(pNode,element);
                return pNode;
			}
            else 
            {
                LogicNode *logicNode = new LogicNode();
                std::string actionName = element->Value();
                if (parseLogic(actionName,logicNode,element))
                {
                    codeRoot->addChild(logicNode);
                }
                return logicNode;
            }
        }
        return NULL;
    }
    void parseAttribute(LogicNode *logic,tinyxml2::XMLElement *element)
    {
        const tinyxml2::XMLAttribute* attr = element->FirstAttribute();
        while (attr != 0)
        {
            //if (strcmp(attr->Name(),"action"))
            {
                logic->propies.propies[std::string(attr->Name())] = attr->Value();
            }
            
            attr = attr->Next();
        }
    }
    bool parseLogic(const std::string &actionName,LogicNode *logic,tinyxml2::XMLElement *element)
    {
        std::vector<std::string> result;
        Util::split(actionName.c_str(),".",result);
        std::string name;
        if (result.size() == 2)
        {
            logic->objectName = result[0];
            name = result[1];
        }
        logic->handler = findHanderByName(name);
        if (!logic->handler) return false;
        #if _DEBUG
            printf("get handler name %s address:%u\n", name.c_str(),logic->handler);
        #endif
        parseAttribute(logic,element);
        return true;
    }
    bool parseVar(VarNode *logic,tinyxml2::XMLElement *element)
    {
        std::string varName = element->Attribute("name");
        std::vector<std::string> result;
        Util::split(varName.c_str(),".",result);
        if (result.size() == 2)
        {
            logic->objectName = result[0];
            varName = result[1];
            logic->handler = static_cast<LogicFieldHandler*>(findHanderByName(varName.c_str()));
        }
        if (!logic->handler)
        {
            parseUserVar(logic,element,varName);
        }
        parseAttribute(logic,element);
        return true;
    }
    bool parseUserVar(LogicNode *logic,tinyxml2::XMLElement *element,const std::string& varName)
    {
        logic->handler = findInnerVarHanderByName(varName);
        if (!logic->handler) {
            const char* type = element->Attribute("type");

            if (type && !strcmp(type,"number"))
            {
                logic->handler = new TypeHandler<double>();
            }
            else if (type && !strcmp(type,"int"))
            {
                logic->handler = new TypeHandler<int>();
            }
            else
            {
                logic->handler = new TypeHandler<std::string>();
            }
            addInnerVarHandler(varName,logic->handler);

            printf("new var %s\n",varName.c_str());
        }
        return true;
    }
};




