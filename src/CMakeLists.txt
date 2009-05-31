PROJECT(MXIT_CLIENT)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${MXIT_CLIENT_SOURCE_DIR} ${MXIT_CLIENT_BINARY_DIR})

# add Qt modules here, the include will setup QT_LIBRARIES
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTWEBKIT TRUE)
INCLUDE(${QT_USE_FILE})

# .cxx sources
SET(MXIT_CLIENT_SRCS_CXX
  common/hash_variables.cpp

  network/packets/http.cpp
  network/packets/tcp.cpp
  
  network/packet.cpp
  network/gateway.cpp
  network/connection.cpp
  
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
  protocol/handlers/29_renamegroup.cpp
  protocol/handlers/30_removegroup.cpp
  protocol/handlers/31_splashscreenclickthrough.cpp
  protocol/handlers/32_setshownpresenceandstatus.cpp
  protocol/handlers/33_blocksubscription.cpp
  protocol/handlers/34_sendmessagetogroup.cpp
  protocol/handlers/41_setmood.cpp
  protocol/handlers/43_loginkick.cpp
  protocol/handlers/44_createnewgroupchat.cpp
  protocol/handlers/45_addnewgroupchatmember.cpp
  protocol/handlers/51_getnewsubscription.cpp
  protocol/handlers/52_allowsubscription.cpp
  protocol/handlers/54_registergateway.cpp
  protocol/handlers/55_denysubscription.cpp
  protocol/handlers/56_unregistergateway.cpp
  protocol/handlers/1k_keepalive.cpp
  
  protocol/aes.cpp
  protocol/handler.cpp
  protocol/handshaker.cpp
  protocol/packet_header.cpp
  
  mxit/address_book.cpp
  mxit/client.cpp
  mxit/contact.cpp
  mxit/conversation.cpp
  mxit/conversations.cpp
  mxit/message.cpp

  gui/theme_components/chat.cpp
  gui/theme_components/contact/base.cpp  
  gui/theme_components/contact/presence.cpp
  gui/theme_components/contact/type.cpp  
  
  gui/dock_widgets/add_contact.cpp
  gui/dock_widgets/contacts.cpp
  gui/dock_widgets/conversations.cpp
  gui/dock_widgets/debug.cpp
  gui/dock_widgets/log.cpp
  gui/dock_widgets/options.cpp
  
  gui/dialogs/start_group_chat.cpp
  gui/dialogs/update_contact_info.cpp
  gui/dialogs/update_group_name.cpp
  gui/dialogs/allow_subscription.cpp
  gui/dialogs/login.cpp
  gui/dialogs/register.cpp
  
  gui/widgets/profile_settings.cpp
  gui/widgets/login_settings.cpp
  gui/emoticon.cpp
  
  gui/mxitc.cpp
  gui/chat_area.cpp
  gui/first_run_wizard.cpp
  gui/conversations_widgets_controller.cpp
  gui/conversations_tab_widget.cpp
  gui/mxit_dialog.cpp
  gui/mxit_dock_widget.cpp
  gui/theme.cpp
  
  main.cpp
)

# files which need to be moc'd by Qt
SET(MXIT_CLIENT_MOC_SRCS
    
  network/connection.h

  protocol/handshaker.h
  
  mxit/address_book.h
  mxit/client.h
  mxit/conversation.h
  mxit/conversations.h
  
  
  gui/dock_widgets/log.h
  gui/dock_widgets/options.h
  gui/dock_widgets/debug.h
  gui/dock_widgets/conversations.h
  gui/dock_widgets/contacts.h
  gui/dock_widgets/add_contact.h
  
  gui/dialogs/start_group_chat.h
  gui/dialogs/update_contact_info.h
  gui/dialogs/update_group_name.h
  gui/dialogs/allow_subscription.h
  gui/dialogs/login.h
  gui/dialogs/register.h
  
  gui/widgets/profile_settings.h
  gui/widgets/login_settings.h
  
  gui/mxitc.h
  gui/chat_area.h
  gui/conversations_widgets_controller.h
  gui/conversations_tab_widget.h
  gui/mxit_dialog.h
  gui/mxit_dock_widget.h
)

SET(MXIT_CLIENT_UI
  
  gui/dock_widgets/add_contact.ui
  gui/dock_widgets/contacts.ui
  gui/dock_widgets/conversations.ui
  gui/dock_widgets/debug.ui
  gui/dock_widgets/log.ui
  gui/dock_widgets/options.ui
  
  gui/dialogs/start_group_chat.ui
  gui/dialogs/update_contact_info.ui
  gui/dialogs/update_group_name.ui
  gui/dialogs/allow_subscription.ui
  gui/dialogs/login.ui
  gui/dialogs/register.ui
  
  gui/widgets/profile_settings.ui
  gui/widgets/login_settings.ui
  
  gui/mxitc.ui
  gui/chat_area.ui
  gui/first_run_wizard.ui
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

