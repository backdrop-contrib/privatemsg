# Privatemsg

Allows your site's visitors to send private messages to each other. Perfect for
community oriented sites, such as forums and social networking sites. Many
additional features are available as separate modules.

The Privatemsg module is designed to be a flexible and powerful system for
sending and receiving internal messages. This includes user-to-user messages,
user-to-role messages, messages from the site administrator, and much more. If
you want some or all users on your site to have their own "mailbox"--and other
users with the proper permissions to be able to message them through this
mailbox--then this is the module for you.

## Features

* send and receive private messages on a site
  (between multiple recipients)
* threaded conversations (make it easier to keep
  track of messages and replies)
* search private messages
* tagging and filtering (helps organise conversations)
* user blocking
* e-mail notifications of new messages

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

## Configuration

1. Go to People > Permissions (admin/people/permissions) and find the relevant
   module permissions underneath the "Private messages" section. If you are not
   logged in as user #1, you must give at least one role (probably the
   administrator role) the "Administer privatemsg" permission to configure this
   module.
2. On this same Permissions page, give at least one role the "Read private
   messages" permission and the "Write new private messages" permission. This
   will allow members of that role to read and write private messages.
3. Go to Configuration > Private messages (`admin/config/messaging/privatemsg`)
   and configure the module settings per your requirements. If you have various
   sub-modules enabled, their settings pages may appear as tabs on this page.
4. Login as a user with the role specified in Step #2. Visit `/messages` to see
   the user's mailbox. Visit `/messages/new` to write a new message.

## Submodules

* Block user messages - allows users to block other users from
  sending them messages.
* Privatemsg Email Notification - notifies users about new
  Private Messages via Email.
* Privatemsg filter - allow users to filter messages using
  tags or other criteria.
* Privatemsg Limits - allows to define limits for private messages
  including amount of sent messages per timeframe and number of
  recipients.
* Privatemsg Realname Integration - provides reverse lookup for
  realnames - dependent on module realname.
* Privatemsg roles - allows to send message to all members of a role.
* Privatemsg Rules Integrations - integrates Rules with Privatemsg,
  allows to act on message events and send private messages -
  dependent on module rules

## Installation

* Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.
* Once installed there is a new item 'Messages' in the User Menu.

## License

This project is GPL v2 software. See the LICENSE.txt file in this
directory for complete text.

## Current Maintainers

* [David Radcliffe](https://github.com/Radcliffe)
* [Herb v/d Dool](https://github.com/herbdool)
* Seeking additional maintainers

## Credits

Initial port to Backdrop by [Graham Oliver](https://github.com/Graham-72), David
Radcliffe and Herb v/d Dool

### Maintainers for Drupal

* leigh (dgtlmoon)
* Sascha Grossenbacher (Berdir)
* litwol
