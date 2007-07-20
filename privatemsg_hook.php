<?php

/**
 * Alter / act on various private message operations
 *
 * @param $message
 *   An object which contains the message. The following properties are provided:
 *     id            A unique ID of the message.
 *    uid           The UID of the author.
 *    name          user name of the author.
 *    author        The UID of the author again.
 *    timestamp     The timestamp when the message was sent.
 *    subject       The subject of the message.
 *    message       The message itself.
 *    format        Format of the message.
 *    newmsg        Is the message unopened?
 *    recipient     The recipient of the message. Either the author or this property is the current user.
 *    folder        The folder which contains the message.
 * @param $op
 *   The operation. Possible values:
 *     prune        The message is permanently removed
 *     delete       The message is marked as deleted.
 *     view         The message is about to be displayed for viewing.
 *     sent         The message was sent. Use a submit handler for the privatemsg_new form to change before sending.
 */
function privatemsgapi_privatemsg($message, $op) {
}
