#include "xmlScript.h"
/**
 * example
 **/
class User{
public:
    int print(Args *stub,Properties &propies)
    {
        printf("%s %u\n", "print",propies.getInt("value"));
        return CodeState::YES;
    }
    int check(Args *stub,Properties &propies)
    {
        if (propies.getInt("state") == 1)
            return CodeState::YES;
        return CodeState::NO;
    }
    int printarg(Args *stub,Properties &propies)
    {
        printf("%s %s\n", "printarg:test=",stub->get<std::string>("test")->c_str());
        return CodeState::YES;
    }
    int a;
    int b;
};

XML_SCRIPT(XmlUserExt){
    BIND_FUNCTION(User,print);
    BIND_FUNCTION(User,printarg);
    BIND_FUNCTION(User,check);
    BIND_FIELD(int,User,b);
    BIND_FIELD(int,User,a);
}

int main()
{
    XmlUserExt ext;

    CodeNode *code = ext.readFromFile("test.xml");
    User user;

    Args args;
    args.add("user",&user);
    
    if (code)
        code->exec(&args); // 执行user 的扩展
    else
        printf("code not exsit\n");
    ext.destroy();
    delete code;

    
}

