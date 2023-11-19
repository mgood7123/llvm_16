add_lldb_library(lldbIntelMPX
  cli-wrapper-mpxtable.cpp

  LINK_LIBS
    liblldbAPI

  LINK_COMPONENTS
    Support
    TargetParser
  )
