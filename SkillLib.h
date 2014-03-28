#pragma once
#include "LLib.h"
class Skill{
public:
	DEC_LANG_FUNCTION(at);
	std::map<int,Args> frames;

	void callback(int frame)
	{
		frames[frame].exec();
	}
};
// skill.at(frame="100"){print("µ±Ç°Ö¡#frame");}
DEC_LANG_FUNCTION(Skill::at)
{
	int frame = propies.get<int>("frame");
	stub->setNumber("frame",frame);
	frames[frame] = *stub;
	return CodeState::NO;
}