<?php
// $Id$

/**
 * @file
 * Admin menu callbacks for privatemsg_filter module.
 */

/**
 * Display a list of tags to administer them.
 */
function privatemsg_tags_admin() {
  drupal_set_title(t('Tags'));

  $header = array(
    'tag' => array(
      'data'    => t('Tag'),
      'field'   => 'tag',
      'class'   => array('privatemsg-filter-header-tag'),
      'sort'    => 'asc',
    ),
    'usage' => array(
      'data'    => t('Usage'),
      'field'   => 'count',
      'class'   => array('privatemsg-filter-header-usage'),
    ),
    'public' => array(
      'data'    => t('Public'),
      'field'   => 'public',
      'class'   => array('privatemsg-filter-header-public'),
    ),
    'operations' => array(
      'data'    => t('Operations'),
      'class'   => array('privatemsg-filter-header-operations'),
    ),
  );
  $query = _privatemsg_assemble_query(array('tags', 'privatemsg_filter'))->extend('PagerDefault')->extend('TableSort');
  $query
    ->orderByHeader($header)
    ->limit();

  $rows = array();
  foreach ($query->execute() as $tag) {
    $row = array();
    $row['tag'] = check_plain($tag->tag);
    $row['count'] = $tag->count;
    $row['public'] = $tag->public ? t('Yes') : '-';
    $row['operations'] = l(t('edit'), 'admin/config/messaging/privatemsg/tags/edit/' . $tag->tag_id);
    $row['operations'] .= ' ' . l(t('delete'), 'admin/config/messaging/privatemsg/tags/delete/' . $tag->tag_id);

    $rows[] = $row;
  }

  if (empty($rows)) {
    // Display a message if now tags are available.
    $rows[] = array(array('data' => t('No tags available.'), 'colspan' => count($header)));
  }
  
  return theme('table',  array('header' => $header, 'rows' =>  $rows)) . theme('pager');
}

/**
 * A form to edit existing or create new tags.
 *
 * @param $form_state Form state array
 * @param $tag_id Tag Id if an edit form should be displayed, NULL for a create
 *   new tag form.
 */
function privatemsg_tags_form($form, &$form_state, $tag_id = NULL) {
  $form = array();

  if ($tag_id) {
    $tag = db_query('SELECT * from {pm_tags} pmt WHERE pmt.tag_id = :tag_id', array(':tag_id' => $tag_id))->fetchAssoc();
    $form['tag_id'] = array(
      '#value' => $tag_id,
      '#type' => 'value',
    );
    drupal_set_title(t('Edit @tag', array('@tag' => $tag['tag'])));
  }

  $form['tag'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => empty($tag_id) ? NULL : $tag['tag'],
  );

  $form['public'] = array(
    '#title' => t('Public'),
    '#type' => 'checkbox',
    '#default_value' => empty($tag_id) ? TRUE : $tag['public'],
    '#description' => t('Public tags are visible for all users, private tags are only visible if they use them.'),
  );

  $form['submit'] = array(
    '#value' => empty($tag_id) ? t('Create tag') : t('Save tag'),
    '#type'  => 'submit',
  );

  return $form;
}

/**
 * Form validate function for tags admin page.
 * 
 * Make sure that a tag name is unique.
 */
function privatemsg_tags_form_validate($form, &$form_state) {
  $tag_id = isset($form_state['values']['tag_id']) ? $form_state['values']['tag_id'] : 0;
  if ($tag_id) {
    // We are editing an existing tag, exclude the current tag from the search.
    $exists = db_query("SELECT 1 FROM {pm_tags} WHERE tag = :tag AND tag_id <> :tag_id", array(
        ':tag' => $form_state['values']['tag'],
        ':tag_id' => $tag_id,
    ))->fetchField();
  }
  else {
    $exists = db_query("SELECT 1 FROM {pm_tags} WHERE tag = :tag", array(
        ':tag' => $form_state['values']['tag'],
    ))->fetchField();
  }
  if ($exists) {
    form_set_error('tag', t('Tag already exists, choose a different name.'));
  }
}

/**
 * Form submit handler for add/edit forms.
 */
function privatemsg_tags_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['tag_id'])) {
    // Tag already exists, update the existing tag.
    db_update('pm_tags')
      ->fields(array(
        'tag' => $form_state['values']['tag'],
        'public' => $form_state['values']['public'],
      ))
      ->condition('tag_id', $form_state['values']['tag_id'])
      ->execute();
    drupal_set_message(t('Tag updated.'));
  }
  else {
    // Tag does not yet exist, create a new one.
    db_insert('pm_tags')
      ->fields(array(
        'tag' => $form_state['values']['tag'],
        'public' => $form_state['values']['public'],
      ))
      ->execute();
    drupal_set_message(t('Tag created.'));
  }
  $form_state['redirect'] = 'admin/config/messaging/privatemsg/tags';
}

/**
 * Confirmation form for deleting tags.
 *
 * @param $tag_id Id of the tag that should be deleted.
 */
function privatemsg_filter_tags_delete($form, &$form_state, $tag_id) {
  $form['tag_id'] = array(
    '#type' => 'value',
    '#value' => $tag_id,
  );
  return confirm_form($form,
    t('Are you sure you want to delete?'),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/messaging/privatemsg/tags',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for the confirm delete form.
 */
function privatemsg_filter_tags_delete_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    db_delete('pm_tags')
      ->condition('tag_id', $form_state['values']['tag_id'])
      ->execute();
    db_delete('pm_tags_index')
      ->condition('tag_id', $form_state['values']['tag_id'])
      ->execute();

    drupal_set_message(t('Tag has been deleted'));
  }
  $form_state['redirect'] = 'admin/config/messaging/privatemsg/tags';
}

function privatemsg_filter_admin() {
  $form = array();

  $form['privatemsg_filter_searchbody'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Search message body'),
    '#description'   => t('WARNING: turning on this feature will slow down search performance by a large factor. Gets worse as your messages database increases.'),
    '#default_value' => variable_get('privatemsg_filter_searchbody', FALSE),
  );

  $form['privatemsg_filter_tagfield_weight'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Position of the tagging textfield'),
    '#description'   => t('Use higher values to push the form lower down the page, lower or negative values to raise it higher.'),
    '#size'          => 4,
    '#default_value' => variable_get('privatemsg_filter_tagfield_weight', 10),
  );

  return system_settings_form($form);
}
