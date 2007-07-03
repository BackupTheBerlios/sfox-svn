SET(SRC glstats.cxx LibraryLoader.cxx GLXFuncs.cxx glx.cxx
        font.cxx timer.cxx GLObjectList.cxx
        GLStatConfig.cxx Global.cxx GLContext.cxx
		    LoggerManager.cxx UIWindow.cxx UILoggerDisplay.cxx
		    CounterManager.cxx)

ADD_LIBRARY(GLStat SHARED ${SRC})
