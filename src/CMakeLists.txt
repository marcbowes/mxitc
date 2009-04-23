PROJECT(MXIT_CLIENT)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${MXIT_CLIENT_SOURCE_DIR} ${MXIT_CLIENT_BINARY_DIR})

# add Qt modules here, the include will setup QT_LIBRARIES
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})

# .cxx sources
SET(MXIT_CLIENT_SRCS_CXX

  network/packets/http.cpp
  network/packets/tcp.cpp
  
  protocol/aes.cpp
  protocol/handler.cpp
  protocol/handlers/01_login.cpp
  protocol/handlers/02_logout.cpp
  protocol/handlers/03_getcontacts.cpp
  
  network/packet.cpp
  network/gateway.cpp
  network/connection.cpp
  protocol/handshaker.cpp
  mxit/client.cpp
  var/contact.cpp
  gui/dialogs/addContact.cpp
  gui/dialogs/login.cpp
  gui/mxitc.cpp
  main.cpp
)

# files which need to be moc'd by Qt
SET(MXIT_CLIENT_MOC_SRCS

  protocol/handler.h
  protocol/handlers/01_login.h
  protocol/handlers/02_logout.h
  protocol/handlers/03_getcontacts.h
  
  network/connection.h
  protocol/handshaker.h
  mxit/client.h
  var/contact.h
  gui/dialogs/addContact.h
  gui/dialogs/login.h
  gui/mxitc.h
)

SET(MXIT_CLIENT_UI
  
  gui/dialogs/login.ui
  gui/dialogs/addContact.ui
  gui/mxitc.ui
)

# build ui_XXX files from the XML-style .ui files
QT4_WRAP_UI(MXIT_CLIENT_SRCS_CXX ${MXIT_CLIENT_UI})
# this moc's the above variable and appends to the cxx sources
QT4_WRAP_CPP(MXIT_CLIENT_SRCS_CXX ${MXIT_CLIENT_MOC_SRCS})

IF(HANDSHAKER_DEBUG)
  ADD_DEFINITIONS("-DHANDSHAKER_DEBUG")
ENDIF(HANDSHAKER_DEBUG)

ADD_EXECUTABLE(
  mxitc
  ${MXIT_CLIENT_SRCS_CXX}
)

TARGET_LINK_LIBRARIES(mxitc ${QT_LIBRARIES})

