<?php
// $Id$

/**
 * @file
 *   Provides a preprocess function on behalf of the privatemsg module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function privatemsg_preprocess_author_pane(&$variables) {
  global $user;
  $account_id = $variables['account']->uid;
  $image_path = $variables['image_path'];

  // Send private message
  if ($account_id != 0 && $account_id != $user->uid && user_access('write privatemsg')) {
    $variables['privatemsg'] =
      l(theme('image', "$image_path/private-message.png",
              t('Send private message'), t('Send private message'), NULL, TRUE),
        "messages/new/$account_id",
        array('absolute' => TRUE, 'html' => TRUE));

    $variables['privatemsg_link'] = l(t('Send PM'), 'messages/new/'. $account_id);
  }
}
