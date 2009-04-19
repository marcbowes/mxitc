PROJECT(MXIT_CLIENT)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${MXIT_CLIENT_SOURCE_DIR} ${MXIT_CLIENT_BINARY_DIR})

# add Qt modules here, the include will setup QT_LIBRARIES
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})

# .cxx sources
SET(MXIT_CLIENT_SRCS_CXX
  
  network/packet.cpp
  network/tcp_packet.cpp
  protocol/handshaker.cpp
  mxit/client.cpp
  mxit/aes.cpp
  gui/login.cpp
  main.cpp
)

# files which need to be moc'd by Qt
SET(MXIT_CLIENT_MOC_SRCS
  
  protocol/handshaker.h
  mxit/client.h
  gui/login.h
)

SET(MXIT_CLIENT_UI
  
  gui/login.ui
)

# build ui_XXX files from the XML-style .ui files
QT4_WRAP_UI(MXIT_CLIENT_SRCS_CXX ${MXIT_CLIENT_UI})
# this moc's the above variable and appends to the cxx sources
QT4_WRAP_CPP(MXIT_CLIENT_SRCS_CXX ${MXIT_CLIENT_MOC_SRCS})

IF(HANDSHAKER_DEBUG)
  add_definitions("-DHANDSHAKER_DEBUG")
ENDIF(HANDSHAKER_DEBUG)

ADD_EXECUTABLE(
  mxitc
  ${MXIT_CLIENT_SRCS_CXX}
)

TARGET_LINK_LIBRARIES(mxitc ${QT_LIBRARIES})

