<?php
// $Id$

/**
 * @file
 * User menu callbacks for pm_block_user.module.
 */

function pm_block_user_form($form, $form_state, $author) {
  global $user;

  $form['author'] = array(
    '#type' => 'value',
    '#value' => $author->uid,
  );
    $form['recipient'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );
  $form['author_name'] = array(
    '#type' => 'value',
    '#value' => $author->name,
  );
  $form['destination'] = array(
    '#type' => 'value',
    '#value' => isset($_GET['destination']) ? $_GET['destination'] : 'messages/',
  );
  $blocked = db_query('SELECT COUNT(recipient) FROM {pm_block_user} WHERE author = :author AND recipient = :recipient', array(
    ':author' => $author->uid,
    ':recipient' => $user->uid,
  ))->fetchField();
  if ($blocked) {
    $form['block_action'] = array(
      '#type' => 'value',
      '#value' => 'unblock_user',
    );
    return confirm_form($form,
      t('You have previously blocked "@author" from sending you any more messages. Are you sure you want to unblock this user?', array('@author' => $author->name)),
      isset($_GET['destination']) ? $_GET['destination'] : 'messages/',
      t('This action cannot be undone.'),
      t('Unblock @author', array('@author' => $author->name)),
      t('Cancel')
    );
  }
  else {
    $form['block_action'] = array(
      '#type' => 'value',
      '#value' => 'block_user',
    );
    return confirm_form($form,
      t('Are you sure you want to block "@author" from sending you any more messages?', array('@author' => $author->name)),
      isset($_GET['destination']) ? $_GET['destination'] : 'messages/',
      '',
      t('Block @author', array('@author' => $author->name)),
      t('Cancel')
    );
  }
}

/**
 * Submit callback for block user confirmation form.
 */
function pm_block_user_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    switch ($form_state['values']['block_action']) {
      case 'block_user':
        db_insert('pm_block_user')
          ->fields(array(
            'author' => $form_state['values']['author'],
            'recipient' => $form_state['values']['recipient'],
          ))
          ->execute();
        drupal_set_message(t('@author has been blocked from sending you any further messages.', array('@author' => $form_state['values']['author_name'])));
      break;
      case 'unblock_user':
        db_delete('pm_block_user')
          ->condition('author', $form_state['values']['author'])
          ->condition('recipient', $form_state['values']['recipient'])
          ->execute();
        drupal_set_message(t('@author is now allowed to send you new messages.', array('@author' => $form_state['values']['author_name'])));
      break;
    }
  }
  $form_state['redirect'] = $form_state['values']['destination'];
}

/**
 * Formbuilder function to build a simple form to block users.
 */
function pm_block_user_list() {
  global $user;

  $form['new'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('Block a user'),
  );
  $form['new']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#autocomplete_path' => 'messages/user-name-autocomplete',
    '#description'        => t('Separate multiple names with commas.'),
  );
  $form['new']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Block user'),
    '#submit' => array('pm_block_user_block_submit'),
  );

  $header = array(
    array(
      'data' => t('Username'),
      'field' => 'u.name',
      'sort'  => 'asc',
      ),
    array(
      'data' => t('Operations'),
      ),
  );

  $select = db_select('pm_block_user', 'pmb')->extend('PagerDefault')->extend('TableSort')
    ->fields('pmb', array('author'))
    ->condition('pmb.recipient', $user->uid)
    ->limit(20)
    ->orderByHeader($header);

  // Only show existing users, therefore join users.
  $select->innerJoin('users', 'u', 'u.uid = pmb.author');

  $rows = array();
  foreach ($select->execute() as $row) {
    $rows[] = array(
      theme('username', array('account' => user_load($row->author))),
      l(t('unblock'), 'messages/block/' . $row->author, array('query' => drupal_get_destination())),
    );
  }
  $form['#header'] = $header;
  $form['#rows'] = $rows;
  return $form;
}

/**
 * Submit callback for block user form.
 */
function pm_block_user_block_submit($form, &$form_state) {
  global $user;
  list($accounts, $invalid) = _privatemsg_parse_userstring($form_state['values']['name']);
  if (!empty($accounts)) {
    $insert = db_insert('pm_block_user')->fields(array('author', 'recipient'));
    foreach ($accounts as $account) {
      $insert->values(array(
        'author' => $account->uid,
        'recipient' => $user->uid,
      ));
      drupal_set_message(t('!author has been blocked from sending you any further messages.', array('!author' => theme('username', array('account' => $account)))));
    }
    $insert->execute();
  }
}

/**
 * Theme function to theme the blocked user listing.
 */
function theme_pm_block_user_list($variables) {
  $form = $variables['form'];
  return drupal_render_children($form) . theme('table', array('header' => $form['#header'], 'rows' =>  $form['#rows'])) . theme('pager');
}