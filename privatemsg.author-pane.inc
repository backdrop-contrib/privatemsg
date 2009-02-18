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
  $image_path = $variables['image_path'];
  // Send private message
  if ($url = privatemsg_get_link($variables['account'])) {
    $img = theme('image', "$image_path/private-message.png", t('Send private message'), t('Send private message'), NULL, TRUE);
    $variables['privatemsg'] = l($img, $url, array('absolute' => TRUE, 'html' => TRUE));
    $variables['privatemsg_link'] = l(t('Send PM'), $url);
  }
}
