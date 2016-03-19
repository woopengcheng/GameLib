#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus//consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/loggingmacros.h>

#include "iostream"

using namespace log4cplus;

const int LOOP_COUNT = 100;

class stream : public log4cplus::tostringstream
{
public:
	~stream()
	{
		LOG4CPLUS_DEBUG(Logger::getRoot(), this->str());
	}
protected:
private:
};

#define gDebugStream stream objStream;objStream

int
main()
{
    log4cplus::initialize ();
    helpers::LogLog::getLogLog()->setInternalDebugging(true);
    SharedAppenderPtr append_1(
        new ConsoleAppender());
    append_1->setName(LOG4CPLUS_TEXT("First"));
    append_1->setLayout( std::auto_ptr<Layout>(new TTCCLayout()) );
    Logger::getRoot().addAppender(append_1);

    Logger root = Logger::getRoot();
//     Logger test = Logger::getInstance(LOG4CPLUS_TEXT("test"));
//     Logger subTest = Logger::getInstance(LOG4CPLUS_TEXT("test.subtest"));

    for(int i=0; i<LOOP_COUNT; ++i) {
        NDCContextCreator _context(LOG4CPLUS_TEXT("loop"));
 //       LOG4CPLUS_DEBUG(root, "Entering loop #" << i);
		{
			gDebugStream<<  "Entering loop #" << i;
		
		}
//		std::cout << i<<std::endl;
	}
	{
		gDebugStream<<  "End" << std::endl; 
	}
	system("pause");


    return 0;
}
