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
    $img = theme('image', array(
      'path'  => "$image_path/private-message.png",
      'alt'   => t('Send private message'),
      'title' => t('Send private message'),
      'getsize' => TRUE
    ));
    $variables['privatemsg'] = l($img, $url, array('absolute' => TRUE, 'html' => TRUE));
    $variables['privatemsg_link'] = l(t('Send PM'). '<span></span>', $url, array('attributes' => array('class' => 'author-privatemsg-icon'), 'html' => TRUE));
  }
}
