<?php
// $Id$

/**
 * @file
 * Theme functions for privatemsg.
 */

function theme_privatemsg_list_field($thread) {
}

function phptemplate_privatemsg_list_field__author($thread) {
  if (empty($thread['author'])) {
    return;
  }
  $authors = _privatemsg_generate_user_array($thread['author']);
  $field = array();
  $field['data'] = _privatemsg_format_participants($authors, 3, TRUE);
  $field['class'] = 'privatemsg-list-author';
  return $field;
}

function phptemplate_privatemsg_list_field__recipient($thread) {
  if (empty($thread['recipient'])) {
    return;
  }
  $recipients = _privatemsg_generate_user_array($thread['recipient']);
  $field = array();
  $field['data'] = _privatemsg_format_participants($recipients, 3, TRUE);
  $field['class'] = 'privatemsg-list-recipient';
  return $field;
}

function phptemplate_privatemsg_list_field__subject($thread) {
  $field = array();
  $field['data'] =l($thread['subject'], 'messages/view/'. $thread['thread_id']);
  $field['class'] = 'privatemsg-list-subject';
  return $field;
}

function phptemplate_privatemsg_list_field__count($thread) {
  $field = array();
  $field['data'] =$thread['count'] -1;
  $field['class'] = 'privatemsg-list-count';
  return $field;
}

function phptemplate_privatemsg_list_field__last_updated($thread) {
  $field = array();
  $field['data'] = format_date($thread['last_updated'], 'small');
  $field['class'] = 'privatemsg-list-date';
  return $field;
}

function phptemplate_privatemsg_list_field__thread_started($thread) {
  $field = array();
  $field['data'] = format_date($thread['thread_started'], 'small');
  $field['class'] = 'privatemsg-list-date-started';
  return $field;
}

function theme_privatemsg_list_header() {

}

function phptemplate_privatemsg_list_header__subject() {
  return array(
    'data'    => t('Subject'),
    'field'   => 'subject',
    'key'     => 'subject',
    'class'   => 'privatemsg-header-subject',
    '#weight' => -40,
  );
}

function phptemplate_privatemsg_list_header__count() {
  return array(
    'data'    => t('Answers'),
    'key'     => 'count',
    'class'   => 'privatemsg-header-count',
    '#weight' => -25,
  );
}

function phptemplate_privatemsg_list_header__author() {
  return array(
    'data'    => t('Authors'),
    'key'     => 'author',
    'class'   => 'privatemsg-header-authors',
    '#weight' => -30,
  );
}

function phptemplate_privatemsg_list_header__recipient() {
  return array(
    'data'    => t('Recipients'),
    'key'     => 'recipient',
    'class'   => 'privatemsg-header-recipients',
    '#weight' => -30,
  );
}

function phptemplate_privatemsg_list_header__last_updated() {
  return array(
    'data'    => t('Last Updated'),
    'field'   => 'last_updated',
    'key'     => 'last_updated',
    'sort'    => 'desc',
    'class'   => 'privatemsg-header-lastupdated',
    '#weight' => -20,
  );
}

function phptemplate_privatemsg_list_header__thread_started() {
  return array(
    'data'    => t('Started'),
    'field'   => 'thread_started',
    'key'     => 'thread_started',
    'class'   => 'privatemsg-header-threadstarted',
    '#weight' => -15,
  );
}

/**
 * Theme to display the privatemsg list
 */
function theme_privatemsg_list($form) {
  $has_posts = !empty($form['#rows']);

  drupal_add_css(drupal_get_path('module', 'privatemsg') .'/styles/privatemsg-list.css');

  // sort the headers array based on the #weight property.
  $headers = $form['#headers'];
  usort($headers, 'element_sort');

  $themed_rows = array();
  // Check if there is atleast a single thread.
  if ($has_posts) {
    foreach ($form['#rows'] as $thread_id => $row) {
      $data = array();
      // Render the checkbox.
      $data[] = drupal_render($form['threads'][$thread_id]);

      // Store the #rows data in the same order as the header is, the key property of the header refers to the field that belongs to it.
      foreach ($headers as $header) {
        if (!empty($header['key'])) {
          if (isset($row['data'][$header['key']])) {
            $data[] = $row['data'][$header['key']];
          }
          else {
            // Store a empty value so that the order is still correct.
            $data[] = '';
          }
        }
      }
      // Replace the data
      $row['data'] = $data;
      $themed_rows[] = $row;
    }
  }
  else {
    // Display a message if now messages are available.
    $themed_rows[] = array(array('data' => t('No messages available.'), 'colspan' => count($headers)));
  }

  // Remove any data in header that we don't need anymore.
  foreach ($headers as $id => $header) {
    unset($headers[$id]['key']);
    unset($headers[$id]['#weight']);
  }

  // Theme the table, pass all generated information to the table theme function.
  $form['list'] = array('#value' => theme('table', $headers, $themed_rows, array('class' => 'privatemsg-list')), '#weight' => 5);
  return drupal_render($form);
}

function theme_privatemsg_new_block($count) {
  $text = format_plural($count, 'You have a new message, click here to read it',
                        'You have @count new messages, click here to read them',
                        array('@count' => $count));

  return l($text, 'messages', array('attributes' => array('id' => 'privatemsg-new-link')));
}