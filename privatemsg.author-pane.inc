<?php
// $Id$

/**
 * @file
 * Provides a preprocess function to integrate Privatemsg with Author Pane.
 */

/**
 * Implements hook_preprocess_author_pane().
 */
function privatemsg_preprocess_author_pane(&$variables) {
  // Check if this preprocess needs to be run given who's calling it.
  if (!author_pane_run_preprocess('privatemsg', $variables['caller'])) {
    return;
  }

  global $user;
  $account = $variables['account'];
  $account_id = $account->uid;
  $account_name = $account->name;

  if ($account_id != $user->uid) {
    $url = privatemsg_get_link($account);
    if (!empty($url)) {
      $variables['privatemsg'] = l(t('Send private message') . '<span></span>', $url, array('attributes' => array('class' => array('author-privatemsg-icon'), 'title' => t('Send @name a private message', array('@name' => $account_name))), 'html' => TRUE));
    }
  }
}

/**
 * Implementation of hook_author_pane_allow_preprocess_disable().
 */
function privatemsg_author_pane_allow_preprocess_disable() {
  return array('privatemsg' => 'Privatemsg');
}
