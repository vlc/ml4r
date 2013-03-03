#ifndef __VLC_MESSAGE_H__
#define __VLC_MESSAGE_H__

#include <iostream>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using std::cout;
using std::endl;

class VlcMessage
{
public: 

	VlcMessage() : indentLevel(0) {}
	~VlcMessage() {}

	void Write(std::string message, int level = 0) 
	{
		for (int i=0; i<indentLevel; ++i)
			cout << "    ";
		if (level > 1) cout << "*** ";
		cout << message;
		if (level > 1) cout << " ***";
		cout << endl;
	}

	void Begin(std::string message)
	{
		cout << message << endl;
		++indentLevel;
	}

	void End()
	{
		cout << endl;
		--indentLevel;
	}

	int indentLevel;
};

extern VlcMessage vlcMessage;

#endif // __VLC_MESSAGE_H__