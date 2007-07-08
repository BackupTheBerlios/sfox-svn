SET(SRC glstats.cxx LibraryLoader.cxx GLXFuncs.cxx glx.cxx
        font.cxx timer.cxx GLObjectList.cxx
        GLStatConfig.cxx GlobalGLStat.cxx GLContext.cxx
	LoggerManager.cxx UIWindow.cxx UILoggerDisplay.cxx
	CounterManager.cxx UIFrame.cxx UIFramedWindow.cxx
	ProxyFunction.cxx GLFuncs.cxx LoggerXml.cxx)

ADD_LIBRARY(GLStat SHARED ${SRC})
