PROJECT(MXIT_CLIENT)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${MXIT_CLIENT_SOURCE_DIR} ${MXIT_CLIENT_BINARY_DIR})

# add Qt modules here, the include will setup QT_LIBRARIES
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})

# .cxx sources
SET(MXIT_CLIENT_SRCS_CXX
  common/hash_variables.cpp

  network/packets/http.cpp
  network/packets/tcp.cpp
  
  protocol/aes.cpp
  protocol/handler.cpp
  protocol/packet_header.cpp
  protocol/handlers/01_login.cpp
  protocol/handlers/02_logout.cpp
  protocol/handlers/03_getcontacts.cpp
  protocol/handlers/05_updatecontactinfo.cpp
  protocol/handlers/06_subscribetoanewcontact.cpp
  protocol/handlers/08_removecontact.cpp
  protocol/handlers/09_getnewmessages.cpp
  protocol/handlers/10_sendnewmessage.cpp
  protocol/handlers/11_register.cpp
  protocol/handlers/27_getmultimediamessage.cpp
  protocol/handlers/32_setshownpresenceandstatus.cpp
  
  network/packet.cpp
  network/gateway.cpp
  network/connection.cpp
  protocol/handshaker.cpp
  mxit/client.cpp
  
  gui/theme/presence.cpp
  
  gui/mxit_dock_widget.cpp
  gui/contact.cpp
  gui/message.cpp
  
  gui/dock_widgets/log.cpp
  gui/dock_widgets/options.cpp
  gui/dock_widgets/debug.cpp
  gui/dock_widgets/contacts.cpp
  
  gui/dialogs/addContact.cpp
  gui/dialogs/login.cpp
  
  gui/contact_metadata.cpp
  gui/mxitc.cpp
  gui/mxit_dialog.cpp
  gui/theme.cpp
  
  main.cpp
)

# files which need to be moc'd by Qt
SET(MXIT_CLIENT_MOC_SRCS
  
  network/connection.h
  protocol/handshaker.h
  mxit/client.h
  
  gui/mxit_dock_widget.h
  gui/message.h
  gui/contact.h
  
  gui/dock_widgets/log.h
  gui/dock_widgets/options.h
  gui/dock_widgets/debug.h
  gui/dock_widgets/contacts.h
  
  gui/dialogs/addContact.h
  gui/dialogs/login.h
  
  gui/contact_metadata.h
  gui/mxitc.h
  gui/mxit_dialog.h
)

SET(MXIT_CLIENT_UI
  
  gui/dock_widgets/log.ui
  gui/dock_widgets/options.ui
  gui/dock_widgets/debug.ui
  gui/dock_widgets/contacts.ui
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

