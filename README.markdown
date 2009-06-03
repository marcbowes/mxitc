# MXit PC Client

mxitc is a client for use on the MXit mobile platform. MXit is essentially an
IM program for a wide variety of cellphones. MXit provides a variety of
services, such as Gateway support for other IM programs (e.g. Google Talk).

Written in C++ using the Qt toolkit, mxitc aims to be a fully featured MXit
client for use on all Qt-capable systems. This includes:

  * Linux
  * Windows
  * Mac OSX

Note that the mxitc theme pack is a [seperate project] [1].

## Features

### Core

  * Login/logut (with autologin option)
  * Registration
  * Automatic keep-alive
  * Supports both TCP and HTTP
  * Supports HTTP Proxy (with username/password)

### Chat

  * Support for links
  * Support for MXit markup (does not handle backslash escaping or resizing)
  * Support for MXit commands
  * Group messaging
  * Emoticons

### Contacts

  * Presence updates
  * Mood updates
  * Online/offline notications
  * Add contacts (with invite messages)
  * Remove contacts
  * Accept/reject subscriptions
  * Hide offline contacts
  * Group users
  * Gateway support

### Multimedia

  * Supports MXit chunked data
  * Supports MXit splash-screen advertising

### Personalization

  * Profile updates
  * Skins and themes
  * Emoticon packs

## License

Released under the GPLv3. Please see LICENSE in the project root folder for more
information.

## Building

Please see the relevant build file in the docs/ folder.

## Issues

Please report all issues on the GitHub [issue tracker for mxitc] [2].
Please do not post feature requests as issues.

## Authors

  * Marc Bowes (marcbowes AT gmail DOT com)
  * Tim Sjoberg (bedlamp AT gmail DOT com)
  * Richard Baxter (richard.johnathan.baxter AT gmail DOT com)

## Using the installer

  Should be straight forward

[1]: http://github.com/marcbowes/mxitc-themes "mxitc-themes"
[2]: http://github.com/marcbowes/mxitc/issues "mxitc issues"
