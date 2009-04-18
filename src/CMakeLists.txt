PROJECT(MXIT_CLIENT)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# add Qt modules here, the include will setup QT_LIBRARIES
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})

# .cxx sources
SET(MXIT_CLIENT_SRCS_CXX
  
  main.cpp
  mxit.cpp
)

# files which need to be moc'd by Qt
SET(MXIT_CLIENT_MOC_SRCS
  
  dialog.h
  mxit.h
)

SET(MXIT_CLIENT_UI
  
  dialog.ui
)

# build ui_XXX files from the XML-style .ui files
QT4_WRAP_UI(MXIT_CLIENT_SRCS_CXX ${MXIT_CLIENT_UI})
# this moc's the above variable and appends to the cxx sources
QT4_WRAP_CPP(MXIT_CLIENT_SRCS_CXX ${MXIT_CLIENT_MOC_SRCS})

ADD_EXECUTABLE(
  mxit
  ${MXIT_CLIENT_SRCS_CXX}
)

TARGET_LINK_LIBRARIES(mxit ${QT_LIBRARIES})

