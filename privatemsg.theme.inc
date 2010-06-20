<?php
// $Id$

/**
 * @file
 * Theme functions for privatemsg.
 */

/**
 * @defgroup theming Theming documentation
 *
 * It is possible to theme every aspect of privatemsg with theme functions.
 *
 * For the thread list, so called theme patterns are used to allow flexible
 * theming of the table and its columns (including columns added by other
 * modules).
 *
 * Three requirements have to be fulfilled so a new column, with data, is
 * displayed in the private message list:
 *  - A field needs to be returned by the list query, see @link sql Query Builder @endlink.
 *  - A header theme pattern needs to exist for the field.
 *  - A field theme pattern needs to exist for the field.
 *
 * For each field in the query, Privatemsg will try to call a theme pattern for
 * the header. That theme function can return a table header definition and
 * has the following structure: phptemplate_privatemsg_header_fieldname.
 * @see theme_privatemsg_list_header()
 *
 * Privatemsg will then do the same for each row, with the field theme pattern.
 * That theme function should return a table field compatible structure, either
 * just a string or an array. The theme function has to have the following
 * name: phptemplate_privatemsg_header_fieldname.
 * @see theme_privatemsg_list_field()
 *
 * To override an already existing theme function, use the following structure:
 * themename_privatemsg_list_field_fieldname. It is not necessary to
 * overwrite the header theme function unless that information needs to be
 * changed too.
 *
 * Modules can use the hook_form_alter() hook to alter the data. The form with
 * id "privatemsg_list" will contain the header, raw and themed field data in
 * the following form:
 * @code
 * $form['#headers']['field_name'] = $header // Array with the header defintion;
 * $form['#data']['thread_id'] = $data // Raw data of that thread
 * $form['#rows']['thread_id'] = $row // Themed fields of that thread
 * @endcode
 * 
 * Note that the information in #data can be used to populate #rows, but it will
 * not be used by the default theme function theme_privatemsg_list().
 *
 */

/**
 * @addtogroup theming
 * @{
 */

/**
 * Default theme function for field theme.
 *
 * To hide all fields that don't have an explicit theme pattern defined, this
 * theme doesn't return anything.
 *
 * @param $thread
 *   Thread row returned by the list query.
 *
 * @return
 *   A theme_table() compatible field definition.
 */
function theme_privatemsg_list_field($thread) {
}

/**
 * Theme the participants field.
 *
 * @see theme_privatemsg_list_field()
 */
function phptemplate_privatemsg_list_field__participants($thread) {
  $participants = _privatemsg_generate_user_array($thread['participants'], -4);
  $field = array();
  $field['data'] = _privatemsg_format_participants($participants, 3, TRUE);
  $field['class'][] = 'privatemsg-list-participants';
  return $field;
}

/**
 * Theme the subject of the thread.
 *
 * @see theme_privatemsg_list_field()
 */
function phptemplate_privatemsg_list_field__subject($thread) {
  $field = array();
  $options = array();
  $is_new = '';
  if (!empty($thread['is_new'])) {
    $is_new = theme_mark(MARK_NEW);
    $options['fragment'] = 'new';
  }
  $field['data'] = l($thread['subject'], 'messages/view/' . $thread['thread_id'], $options) . $is_new;
  $field['class'][] = 'privatemsg-list-subject';
  return $field;
}

/**
 * Theme the replies field.
 *
 * @see theme_privatemsg_list_field()
 */
function phptemplate_privatemsg_list_field__count($thread) {
  $field = array();
  $field['data'] = $thread['count'];
  $options = array();
  if (!empty($thread['is_new']) && $thread['is_new'] < $thread['count']) {
    $options['fragment'] = 'new';
    $field['data'] .= '<br />' . l((format_plural($thread['is_new'], '(1 new)', '(@count new)')),'messages/view/' . $thread['thread_id'], $options);
  }
  $field['class'][] = 'privatemsg-list-count';
  return $field;
}

/**
 * Theme the last updated column.
 *
 * @see theme_privatemsg_list_field()
 */
function phptemplate_privatemsg_list_field__last_updated($thread) {
  $field = array();
  $field['data'] = format_date($thread['last_updated'], 'small');
  $field['class'][] = 'privatemsg-list-date';
  return $field;
}

/**
 * Theme the thread started column.
 *
 * @see theme_privatemsg_list_field()
 */
function phptemplate_privatemsg_list_field__thread_started($thread) {
  $field = array();
  $field['data'] = format_date($thread['thread_started'], 'small');
  $field['class'][] = 'privatemsg-list-date-started';
  return $field;
}

/**
 * Define the table header for a specific column.
 *
 * This default theme function is used to ignore columns that should not be
 * displayed. Only columns with a specific theme pattern function are displayed.
 *
 * @return
 *   A theme_table() compatible table header definition. Additionally, the key
 *   "key" should be used to specify which row column should be displayed in
 *   this column.
 */
function theme_privatemsg_list_header() {
}

/**
 * Define the subject header.
 *
 * @see theme_privatemsg_list_header()
 */
function phptemplate_privatemsg_list_header__subject() {
  return array(
    'data'    => t('Subject'),
    'field'   => 'subject',
    'class'   => array('privatemsg-header-subject'),
    '#weight' => -40,
  );
}

/**
 * Define the answers column.
 *
 * @see theme_privatemsg_list_header()
 */
function phptemplate_privatemsg_list_header__count() {
  return array(
    'data'    => t('Messages'),
    'class'   => array('privatemsg-header-count'),
    '#weight' => -25,
  );
}

/**
 * Define the participants column.
 *
 * @see theme_privatemsg_list_header()
 */
function phptemplate_privatemsg_list_header__participants() {
  return array(
    'data'    => t('Participants'),
    'class'   => array('privatemsg-header-participants'),
    '#weight' => -30,
  );
}

/**
 * Define the last updated column.
 *
 * @see theme_privatemsg_list_header()
 */
function phptemplate_privatemsg_list_header__last_updated() {
  return array(
    'data'    => t('Last Updated'),
    'field'   => 'last_updated',
    'sort'    => 'desc',
    'class'   => array('privatemsg-header-lastupdated'),
    '#weight' => -20,
  );
}

/**
 * Define the thread started column.
 *
 * @see theme_privatemsg_list_header()
 */
function phptemplate_privatemsg_list_header__thread_started() {
  return array(
    'data'    => t('Started'),
    'field'   => 'thread_started',
    'class'   => array('privatemsg-header-threadstarted'),
    '#weight' => -15,
  );
}

/**
 * Theme a block which displays the number of new messages a user has.
 */
function theme_privatemsg_new_block($count) {
  $count = $count['count'];
  $text = format_plural($count, 'You have a new message, click here to read it',
                        'You have @count new messages, click here to read them',
                        array('@count' => $count));

  return l($text, 'messages', array('attributes' => array('id' => 'privatemsg-new-link')));
}

/**
 * @}
 */
