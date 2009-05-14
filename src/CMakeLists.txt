PROJECT(MXIT_CLIENT)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${MXIT_CLIENT_SOURCE_DIR} ${MXIT_CLIENT_BINARY_DIR})

# add Qt modules here, the include will setup QT_LIBRARIES
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})

# .cxx sources
SET(MXIT_CLIENT_SRCS_CXX
  common/chat_session.cpp
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
  protocol/handlers/12_updateprofile.cpp
  protocol/handlers/17_polldifference.cpp
  protocol/handlers/26_getprofile.cpp
  protocol/handlers/27_getmultimediamessage.cpp
  protocol/handlers/32_setshownpresenceandstatus.cpp
  protocol/handlers/33_blocksubscription.cpp
  protocol/handlers/43_loginkick.cpp
  protocol/handlers/51_getnewsubscription.cpp
  protocol/handlers/52_allowsubscription.cpp
  protocol/handlers/55_denysubscription.cpp
  protocol/handlers/1k_keepalive.cpp
  
  network/packet.cpp
  network/gateway.cpp
  network/connection.cpp
  protocol/handshaker.cpp
  mxit/address_book.cpp
  mxit/client.cpp
  mxit/contact.cpp
  mxit/conversation.cpp
  mxit/message.cpp

  gui/theme_components/contact/base.cpp  
  gui/theme_components/contact/presence.cpp
  gui/theme_components/contact/type.cpp  
  
  gui/dock_widgets/log.cpp
  gui/dock_widgets/options.cpp
  gui/dock_widgets/debug.cpp
  gui/dock_widgets/chat_sessions.cpp
  gui/dock_widgets/contacts.cpp
  gui/dock_widgets/add_contact.cpp
  gui/dock_widgets/conversations.cpp
  
  gui/dialogs/login.cpp
  gui/dialogs/allow_subscription.cpp
  
  gui/mxit_dock_widget.cpp
  
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
  
  gui/dock_widgets/log.h
  gui/dock_widgets/options.h
  gui/dock_widgets/debug.h
  gui/dock_widgets/chat_sessions.h
  gui/dock_widgets/contacts.h
  gui/dock_widgets/add_contact.h
  gui/dock_widgets/conversations.h
  
  gui/dialogs/login.h
  gui/dialogs/allow_subscription.h
  
  gui/mxit_dock_widget.h
  
  gui/mxitc.h
  gui/mxit_dialog.h
)

SET(MXIT_CLIENT_UI
  
  gui/dock_widgets/log.ui
  gui/dock_widgets/options.ui
  gui/dock_widgets/debug.ui
  gui/dock_widgets/chat_sessions.ui
  gui/dock_widgets/contacts.ui
  gui/dock_widgets/add_contact.ui
  
  gui/dialogs/login.ui
  gui/dialogs/allow_subscription.ui
  gui/mxitc.ui
)

# build ui_XXX files from the XML-style .ui files
QT4_WRAP_UI(MXIT_CLIENT_SRCS_CXX ${MXIT_CLIENT_UI})
# this moc's the above variable and appends to the cxx sources
QT4_WRAP_CPP(MXIT_CLIENT_SRCS_CXX ${MXIT_CLIENT_MOC_SRCS})

ADD_EXECUTABLE(
  mxitc
  ${MXIT_CLIENT_SRCS_CXX}
)

TARGET_LINK_LIBRARIES(mxitc ${QT_LIBRARIES})

