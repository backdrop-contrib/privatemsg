# Privatemsg

Privatemsg allows your site's visitors to send private messages
to each other. Perfect for community oriented sites, such as forums
and social networking sites. Many additional features are available
as separate modules.


## Status

This is a port to Backdrop of the Drupal module, version 7.x-1.4.
It is currently under development.

**Warning**: This module has major bugs, and it may crash
your website. Do not use on production websites.

## Features

    + send and receive private messages on a site
      (between multiple recipients)
    + threaded conversations (make it easier to keep
      track of messages and replies)
    + search private messages
    + tagging and filtering (helps organise conversations)
    + user blocking
    + e-mail notifications of new messages

The Privatemsg module allows users to send messages to each other
without having to share their e-mail addresses. Once the module
has been enabled, an inbox link will appear in the navigation menu.
"Write to author" links are included in posts, allowing users to write
private messages instead of commenting openly. Allowing users to
communicate directly with each other is an important part of
community building.

Optionally, users can select whether to receive email notices of new
messages by editing their user profiles. The contacts list contains
only users that you have previously messaged. To contact users not in
your list, you need to know their Backdrop usernames. Administrators
can set messaging options such as frequency of emails, message status
display, and number of messages to display per page. They can also
configure 'Write to Author' options.

## Submodules

  + Block user messages - allows users to block other users from
    sending them messages.

  + Privatemsg Email Notification - notifies users about new
    Private Messages via Email.

  + Privatemsg filter - allow users to filter messages using
    tags or other criteria.

  + Privatemsg Limits - allows to define limits for private messages
    including amount of sent messages per timeframe and number of
    recipients.

  + Privatemsg Realname Integration - provides reverse lookup for
    realnames - dependent on module realname.

  + Privatemsg roles - allows to send message to all members of a role.

  + Privatemsg Rules Integrations - integrates Rules with Privatemsg,
    allows to act on message events and send private messages -
    dependent on module rules


## Installation

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.

- Once installed there is a new item 'Messages' in the User Menu.

## License

This project is GPL v2 software. See the LICENSE.txt file in this
directory for complete text.

## Current Maintainers

+ David Radcliffe (https://github.com/Radcliffe)
+ Seeking additional maintainers

## Credits

### Port to Backdrop

+ Graham Oliver (github.com/Graham-72)

### Maintainers for Drupal:

+ leigh (dgtlmoon)
+ Sascha Grossenbacher (Berdir)
+ litwol


