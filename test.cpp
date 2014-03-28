#include "LLib.h"
#include "SkillLib.h"
class SceneNpc{
	public:
		int talk(Args *stub,Properties &propies)
		{
			stub->setNumber("index",1000);
			//printf("%s %s\n", "print",propies.getString("value").c_str());
			this->stub = *stub;
			return CodeState::NO;
		}
		void call()
		{
			stub.exec();
		}
		
		Args stub;
		SceneNpc()
		{
		
		}
		int a;
};

LANG_SCRIPT(NPCLIB)
{
	BIND_FUNCTION(SceneNpc,talk);
	BIND_FIELD(int,SceneNpc,a);
	BIND_FUNCTION_NAME(SceneNpc,talk,"Ëµ»°");
}
LANG_SCRIPT(SKILLLIB)
{
	BIND_FUNCTION(Skill,at);
}
XML_SCRIPT(NPCLIB1)
{
	BIND_FUNCTION(SceneNpc,talk);
	BIND_FIELD(int,SceneNpc,a);
}


int main()
{
	/*lang::NodeFile lang;
	lang.read("tag.ai");
*/
	theLang.readFromFile("test.ai");
	Args args;
	SceneNpc *npc = new SceneNpc();
	args.add("¹ÖÎï",npc);
	theLang.exec("start",&args);
	Skill * s= new Skill();
	args.add("skill",s);
	theLang.exec("init",&args);
	
	npc->call();

	theXml.readFromFile("test.xml");
	theXml.exec("start",&args);
	
	s->callback(1);
}