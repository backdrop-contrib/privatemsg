<?php
// $Id$

/**
 * @file
 * User menu callbacks for Privatemsg.
 */


/**
 * Returns a form which handles and displays thread actions.
 *
 * Additional actions can be added with the privatemsg_thread_operations hook.
 * It is also possible to extend this form with additional buttons or other
 * elements, in that case, the definitions in the above hook need no label tag,
 * instead, the submit button key needs to match with the key of the operation.
 *
 * @see hook_privatemsg_thread_operations()
 *
 * @return
 *   The FAPI definitions for the thread action form.
 */
function _privatemsg_action_form() {
  $form = array(
      '#type'        => 'fieldset',
      '#title'       => t('Actions'),
      '#prefix'      => '<div class="container-inline">',
      '#suffix'      => '</div>',
      '#collapsible' => TRUE,
      '#collapsed'   => FALSE,
      '#weight'      => 15,
  );
  if (privatemsg_user_access('delete privatemsg')) {
    $form['delete'] = array(
        '#type'   => 'submit',
        '#value'  => t('Delete'),
    );
  }
  // Display all operations which have a label.
  $options = array(0 => t('More actions...'));
  foreach (module_invoke_all('privatemsg_thread_operations') as $operation => $array) {
    if (isset($array['label'])) {
      $options[$operation] = $array['label'];
    }
  }
  $form['operation'] = array(
      '#type'          => 'select',
      '#options'       => $options,
      '#default_value' => 0,
      // Execute the submit button if a operation has been selected.
      '#attributes'    => array('onchange' => "(function ($) { $('#edit-submit').click() })(jQuery);"),
  );
  $form['submit'] = array(
      '#prefix'     => '<div class="privatemsg-op-button">',
      '#suffix'    => '</div>',
      '#type'       => 'submit',
      '#value'      => t('Execute'),
      '#submit'     => array('privatemsg_list_submit'),
  );
  // JS for hiding the execute button.
  drupal_add_js(drupal_get_path('module', 'privatemsg') .'/privatemsg-list.js');
  return $form;
}


function privatemsg_delete($form, $form_state, $thread, $message) {
  $form['pmid'] = array(
    '#type' => 'value',
    '#value' => $message->mid,
  );
  $form['delete_destination'] = array(
    '#type' => 'value',
    '#value' => count($thread['messages']) > 1 ? 'messages/view/' . $message->thread_id : 'messages',
  );

  if (privatemsg_user_access('read all private messages')) {
    $form['delete_options'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete this message for all users?'),
      '#description' => t('Tick the box to delete the message for all users.'),
      '#default_value' => FALSE,
    );
  }
  return confirm_form($form,
    t('Are you sure you want to delete this message?'),
    isset($_GET['destination']) ? $_GET['destination'] : 'messages/view/'. $message->thread_id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function privatemsg_delete_submit($form, &$form_state) {
  global $user;
  $account = clone $user;

  if ($form_state['values']['confirm']) {
    if (isset($form_state['values']['delete_options']) && $form_state['values']['delete_options']) {
      privatemsg_message_change_delete($form_state['values']['pmid'], 1);
      drupal_set_message(t('Message has been deleted for all users.'));
    }
    else {
      privatemsg_message_change_delete($form_state['values']['pmid'], 1, $account);
      drupal_set_message(t('Message has been deleted.'));
    }
  }
  $form_state['redirect'] = $form_state['values']['delete_destination'];
}


/**
 * List messages.
 *
 * @param $form_state
 *   Form state array
 * @param $argument
 *   An argument to pass through to the query builder.
 * @param $uid
 *   User id messages of another user should be displayed
 *
 * @return
 *   Form array
 */
function privatemsg_list($form, &$form_state, $argument = 'list', $uid = NULL) {
  global $user;

  // Setting default behavior...
  $account = $user;
  // Because uid is submitted by the menu system, it's a string not a integer.
  if ((int)$uid > 0 && $uid != $user->uid) {
    // Trying to view someone else's messages...
    if (!privatemsg_user_access('read all private messages')) {
      drupal_set_message(t("You do not have sufficient rights to view someone else's messages"), 'warning');
    }
    elseif ($account_check = _privatemsg_user_load($uid)) {
      // Has rights and user_load return an array so user does exist
      $account = $account_check;
    }
  }
  // By this point we have figured out for which user we are listing messages and now it is safe to use $account->uid in the listing query.

  drupal_add_css(drupal_get_path('module', 'privatemsg') .'/styles/privatemsg-list.css');

  // Load the table columns.
  $columns = array_merge(array('subject', 'last_updated'), array_filter(variable_get('privatemsg_display_fields', array('participants'))));

  // Load the themed list headers based on the available data.
  $headers = _privatemsg_list_headers($columns);
  uasort($headers, 'element_sort');

  $form = array('#list_argument' => $argument);
  $form['list'] = array(
    '#type' => 'tableselect',
    '#header' => $headers,
    '#options' => array(),
    '#attributes' => array('class' => array('privatemsg-list')),
    '#empty' => t('No messages available.'),
    '#weight' => 5,
    '#pre_render' => array('_privatemsg_list_thread'),
  );

  $query = _privatemsg_assemble_query('list', $account, $argument);
  foreach ($query->execute() as $row) {
    // Store the raw row data.
    $form['list']['#options'][$row->thread_id] = (array)$row;
  }

  if (!empty($form['list']['#options'])) {
    $form['actions'] = _privatemsg_action_form();
  }

  // Save the currently active account, used for actions.
  $form['account'] = array('#type' => 'value', '#value' => $account);

  // Define checkboxes, pager and theme
  $form['pager'] = array('#theme' => 'pager', '#weight' => 20);
  //$form['#theme'] = 'privatemsg_list';

  // Store the account for which the threads are displayed.
  $form['#account'] = $account;
  return $form;
}


/**
 * Process privatemsg_list form submissions.
 *
 * Execute the chosen action on the selected messages. This function is
 * based on node_admin_nodes_submit().
 */
function privatemsg_list_submit($form, &$form_state) {
  // Load all available operation definitions.
  $operations = module_invoke_all('privatemsg_thread_operations');

  // Default "default" operation, which won't do anything.
  $operation = array('callback' => 0);

  // Check if a valid operation has been submitted.
  if (isset($form_state['values']['operation']) && isset($operations[$form_state['values']['operation']])) {
    $operation = $operations[$form_state['values']['operation']];
  }

  // Load all keys where the value is the current op.
  $keys = array_keys($form_state['values'], $form_state['values']['op']);

  // Loop over them and detect if a matching button was pressed.
  foreach ($keys as $key) {
    if ($key != 'op' && isset($operations[$key])) {
      $operation = $operations[$key];
    }
  }

  // Only execute something if we have a valid callback and at least one checked thread.
  if (!empty($operation['callback'])) {
    privatemsg_operation_execute($operation, $form_state['values']['list'], $form_state['values']['account']);
  }
}


function privatemsg_new($form, &$form_state, $recipients = array(), $subject = '', $thread_id = NULL, $read_all = FALSE) {
  global $user;

  $recipients_string = '';
  $body      = '';

  // convert recipients to array of user objects
  if (!empty($recipients) && is_string($recipients) || is_int($recipients)) {
    $recipients = _privatemsg_generate_user_array($recipients);
  }
  elseif (is_object($recipients)) {
    $recipients = array($recipients);
  }
  elseif (empty($recipients) && is_string($recipients)) {
    $recipients = array();
  }

  $usercount = 0;
  $to = array();
  $blocked = FALSE;
  foreach ($recipients as $recipient) {
    if ($recipient->type == 'hidden') {
      continue;
    }
    if (isset($to[privatemsg_recipient_key($recipient)])) {
      // We already added the recipient to the list, skip him.
      continue;
    }
    if (!privatemsg_recipient_access($recipient->type, 'write', $recipient)) {
      // User does not have access to write to this recipient, continue.
      continue;
    }

    // Check if another module is blocking the sending of messages to the recipient by current user.
    $user_blocked = module_invoke_all('privatemsg_block_message', $user, array(privatemsg_recipient_key($recipient) => $recipient));
    if (!count($user_blocked) <> 0 && $recipient->recipient) {
      if ($recipient->type == 'user' && $recipient->recipient == $user->uid) {
        $usercount++;
        // Skip putting author in the recipients list for now.
        continue;
      }
      $to[privatemsg_recipient_key($recipient)] = privatemsg_recipient_format($recipient);
    }
    else {
      // Recipient list contains blocked users.
      $blocked = TRUE;
    }
  }

  if (empty($to) && $usercount >= 1 && !$blocked) {
    // Assume the user sent message to own account as if the usercount is one or less, then the user sent a message but not to self.
    $to['user_' . $user->uid] = privatemsg_recipient_format($user);
  }

  if (!empty($to)) {
    $recipients_string = implode(', ', $to);
  }
  if (isset($form_state['values'])) {
    if (isset($form_state['values']['recipient'])) {
      $recipients_string = $form_state['values']['recipient'];

    }
    $subject   = $form_state['values']['subject'];
    $body      = $form_state['values']['body'];
  }
  if (!$thread_id && !empty($recipients_string)) {
    drupal_set_title(t('Write new message to @recipient', array('@recipient' => $recipients_string)));
  } elseif (!$thread_id) {
    drupal_set_title(t('Write new message'));
  }

  $form = array(
    '#type'     => 'fieldset',
    '#access'   => privatemsg_user_access('write privatemsg'),
  );
  if (isset($form_state['privatemsg_preview'])) {
    $form['message_header'] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('preview')),
      '#weight' => -20,
    );
    $form['message_header']['message_preview'] = $form_state['privatemsg_preview'];
  }
  $form['author'] = array(
    '#type' => 'value',
    '#value' => $user,
  );
  if (is_null($thread_id)) {
    $description_array = array();
    foreach (privatemsg_recipient_get_types() as $name => $type) {
      if (privatemsg_recipient_access($name, 'write')) {
        $description_array[] = $type['description'];
      }
    }
    $description = t('Enter the recipient, separate recipients with commas.');
    $description .= theme('item_list', $description_array);

    $form['recipient'] = array(
      '#type'               => 'textfield',
      '#title'              => t('To'),
      '#description'        => $description,
      '#default_value'      => $recipients_string,
      '#required'           => TRUE,
      '#weight'             => -10,
      '#size'               => 50,
      '#autocomplete_path'  => 'messages/user-name-autocomplete',
      // Do not hardcode #maxlength, make it configurable by number of recipients, not their name length.
    );
  }
  $form['subject'] = array(
    '#type'               => 'textfield',
    '#title'              => t('Subject'),
    '#size'               => 50,
    '#maxlength'          => 255,
    '#default_value'      => $subject,
    '#weight'             => -5,
  );

  // The input filter widget looses the format during preview, specify it
  // explicitly.
  if (isset($form_state['values']) && array_key_exists('format', $form_state['values'])) {
    $format = $form_state['values']['format'];
  }
  $form['body'] = array(
    '#type'               => 'text_format',
    '#title'              => t('Message'),
    '#rows'               => 6,
    '#weight'             => 0,
    '#default_value'      => $body,
    '#resizable'          => TRUE,
    '#format'             => isset($format) ? $format : NULL,
  );
  $form['preview'] = array(
    '#type'               => 'submit',
    '#value'              => t('Preview message'),
    '#submit'             => array('privatemsg_new_preview'),
    '#weight'             => 10,
  );
  $form['submit'] = array(
    '#type'               => 'submit',
    '#value'              => t('Send message'),
    '#weight'             => 15,
  );
  $url = 'messages';
  $title = t('Cancel');
  if (isset($_REQUEST['destination'])) {
    $url = $_REQUEST['destination'];
  }
  elseif (!is_null($thread_id)) {
    $url = $_GET['q'];
    $title = t('Clear');
  }

  $form['cancel'] = array(
    '#value'              => l($title, $url, array('attributes' => array('id' => 'edit-cancel'))),
    '#weight'             => 20,
  );

  if (!is_null($thread_id)) {
    $form['thread_id'] = array(
      '#type' => 'value',
      '#value' => $thread_id,
    );
    $form['subject'] = array(
          '#type' => 'value',
          '#default_value' => $subject,
    );
    $recipients_string_themed = implode(', ', $to);
    $form['recipient_display'] = array(
      '#markup' =>  '<p>'. t('<strong>Reply to thread</strong>:<br /> Recipients: !to', array('!to' => $recipients_string_themed)) .'</p>',
      '#weight' => -10,
    );
    if (empty($recipients_string)) {
      // If there are no valid recipients, unset the message reply form.
      $form['#access'] = FALSE;
    }
  }
  $form['read_all'] = array(
    '#type'  => 'value',
    '#value' => $read_all,
  );
  // Attach field widgets.
  $message = (object) array();
  if (isset($form_state['validate_built_message'])) {
    $message = $form_state['validate_built_message'];
  }
  field_attach_form('privatemsg_message', $message, $form, $form_state);
  return $form;
}

function privatemsg_new_validate($form, &$form_state) {
  // The actual message that is being sent, we create this during validation and
  // pass to submit to send out.
  $message = (object)$form_state['values'];
  $message->mid       = 0;
  $message->format    = $message->body['format'];
  $message->body      = $message->body['value'];
  $message->timestamp = time();

  $trimed_body = trim(truncate_utf8(strip_tags($message->body), 50, TRUE, TRUE));
  if (empty($message->subject) && !empty($trimed_body)) {
    $message->subject = $trimed_body;
  }
  // Only parse the user string for a new thread.
  if (!isset($message->thread_id)) {
    list($message->recipients, $invalid) = _privatemsg_parse_userstring($message->recipient);
  }
  else {
    // Load participants. Limit recipients to visible unless read_all is TRUE.
    $message->recipients = _privatemsg_load_thread_participants($message->thread_id, $message->read_all ? FALSE : $message->author);
  }

  $validated = _privatemsg_validate_message($message, TRUE, !isset($message->thread_id));
  foreach ($validated['messages'] as $type => $texts) {
    foreach ($texts as $text) {
      drupal_set_message($text, $type);
    }
  }
  $form_state['validate_built_message'] = $message;
  if (!empty($invalid)) {
    drupal_set_message(t('The following users will not receive this private message: !invalid', array('!invalid' => implode(", ", $invalid))), 'error');
  }
}

function privatemsg_new_preview($form, &$form_state) {
  $message = $form_state['validate_built_message'];
  // Execute submit hook, removes empty fields.
  field_attach_submit('privatemsg_message', $message, $form, $form_state);
  // Load information attached to the message. Use an internal function
  // to avoid the internal field cache.
  _field_invoke_multiple('load', 'privatemsg_message', array($message->mid => $message));
  $form_state['privatemsg_preview'] = array(
    '#markup' => theme('privatemsg_view', array('message' => $message)),
  );
  // This forces the form to be rebuilt instead of being submitted.
  $form_state['rebuild'] = TRUE;
}


/**
 * Submit callback for the privatemsg_new form.
 */
function privatemsg_new_submit($form, &$form_state) {
  $message = $form_state['validate_built_message'];
  field_attach_submit('privatemsg_message', $message, $form, $form_state);

  // Format each recipient.
  $recipient_names = array();
  foreach ($message->recipients as $recipient) {
    $recipient_names[] = privatemsg_recipient_format($recipient);
  }
  try {
    $message = _privatemsg_send($message);
    _privatemsg_handle_recipients($message->mid, $message->recipients);
    drupal_set_message(t('A message has been sent to !recipients.', array('!recipients' => implode(', ', $recipient_names))));
  }
  catch (Exception $e) {
    if (error_displayable()) {
      require_once DRUPAL_ROOT . '/includes/errors.inc';
      $variables = _drupal_decode_exception($e);
      drupal_set_message(t('Failed to send a message to !recipients. %type: %message in %function (line %line of %file).', array('!recipients' => implode(', ', $recipient_names)) + $variables), 'error');
    }
    else {
      drupal_set_message(t('Failed to send a message to !recipients. Contact your site administrator.', array('!recipients' => implode(', ', $recipient_names))), 'error');
    }
  }
}


/**
 * Menu callback for messages/undo/action.
 *
 * This function will test if an undo callback is stored in SESSION and execute it.
 */
function privatemsg_undo_action() {
  // Check if a undo callback for that user exists.
  if (isset($_SESSION['privatemsg']['undo callback']) && is_array($_SESSION['privatemsg']['undo callback'])) {
    $undo = $_SESSION['privatemsg']['undo callback'];
    // If the defined undo callback exists, execute it
    if (isset($undo['function']) && isset($undo['args'])) {
      // Load the user object.
      if (isset($undo['args']['account']) && $undo['args']['account'] > 0) {
        $undo['args']['account'] = _privatemsg_user_load((int)$undo['args']['account']);
      }
      call_user_func_array($undo['function'], $undo['args']);
    }
  }
  // Return back to the site defined by the destination GET param.
  drupal_goto();
}


/**
 * Return autocomplete results for usernames.
 *
 * Prevents usernames from being used and/or suggested twice.
 */
function privatemsg_user_name_autocomplete($string) {
  $names = array();
  // 1: Parse $string and build list of valid user names.
  $fragments = explode(',', $string);
  foreach ($fragments as $index => $name) {
    if ($name = trim($name)) {
      $names[$name] = $name;
    }
  }
  // 2: Find the next user name suggestion.
  $fragment = array_pop($names);
  $matches = array();
  if (!empty($fragment)) {
    $remaining = 10;
    $types = privatemsg_recipient_get_types();
    foreach ($types as $name => $type) {
      if (isset($type['autocomplete']) && is_callable($type['autocomplete']) && privatemsg_recipient_access($name, 'write')) {
        $function = $type['autocomplete'];
        $return = $function($fragment, $names, $remaining);
        if (is_array($return)) {
          $matches += $return;
        }
        $remaining = 10 - count($matches);
        if ($remaining <= 0) {
          break;
        }
      }
    }
  }
  // Prefix the matches and convert them to the correct form for the
  // autocomplete.
  $prefix = count($names) ? implode(", ", $names) .", " : '';
  $suggestions = array();
  foreach ($matches as $match) {
    $suggestions[$prefix . $match . ', '] = $match;
  }

  // convert to object to prevent drupal bug, see http://drupal.org/node/175361
  drupal_json_output((object)$suggestions);
}


/**
 * Menu callback for viewing a thread.
 *
 * @param $thread
 *   A array containing all information about a specific thread, generated by
 *   privatemsg_thread_load().
 * @return
 *   The page content.
 * @see privatemsg_thread_load().
 */
function privatemsg_view($thread) {
  drupal_set_title($thread['subject']);

  // Generate paging links.
  $older = '';
  if (isset($thread['older_start'])) {
    $options = array(
      'query' => array('start' => $thread['older_start']),
      'title' => t('Display older messages'),
    );
     $older = l(t('<<'), 'messages/view/' . $thread['thread_id'], $options);
  }
  $newer = '';
  if (isset($thread['newer_start'])) {
    $options = array(
      'query' => array('start' => $thread['newer_start']),
      'title' => t('Display newer messages'),
    );
    $newer = l(t('>>'), 'messages/view/' . $thread['thread_id'], $options);
  }
  $substitutions = array('@from' => $thread['from'], '@to' => $thread['to'], '@total' => $thread['message_count'], '!previous_link' => $older, '!newer_link' => $newer);
  $title = t('!previous_link Displaying messages @from - @to of @total !newer_link', $substitutions);
  $content['pager_top'] = array(
    '#markup'  => trim($title),
    '#prefix' => '<div class="privatemsg-view-pager">',
    '#suffix' => '</div>',
    '#weight' => -10,
  );

  // Display a copy at the end.
  $content['pager_bottom'] = $content['pager_top'];
  $content['pager_bottom']['#weight'] = 3;

  // Render the participants.
  $content['participants'] = array(
    '#markup' => theme('privatemsg_recipients', array('thread' => $thread)),
    '#weight' => -5
  );

  // Render the messages.
  $content['messages']['#weight'] = 0;
  foreach ($thread['messages'] as $pmid => $message) {
    // Set message as read and theme it.
    if (!empty($message->is_new)) {
      privatemsg_message_change_status($pmid, PRIVATEMSG_READ, $thread['user']);
    }
    $content['messages'][$pmid] = array(
      '#markup' => theme('privatemsg_view', array('message' => $message)),
    );
  }

  // Display the reply form if user is allowed to use it.
  if (privatemsg_user_access('write privatemsg')) {
    $content['reply'] = drupal_get_form('privatemsg_new', $thread['participants'], $thread['subject'], $thread['thread_id'], $thread['read_all']);
    $content['reply']['#weight'] = 5;
  }
  // Check after calling the privatemsg_new form so that this message is only
  // displayed when we are not sending a message.
  if ($thread['read_all']) {
    // User has permission to read all messages AND is not a participant of the current thread.
    drupal_set_message(t('This conversation is being viewed with escalated privileges and may not be the same as shown to normal users.'), 'warning');
  }

  return $content;
}

/**
 * Batch processing function for rebuilding the index.
 */
function privatemsg_load_recipients($mid, $recipient, &$context) {
  // Get type information.
  $type = privatemsg_recipient_get_type($recipient->type);

  // First run, initialize sandbox.
  if (!isset($context['sandbox']['current_offset'])) {
    $context['sandbox']['current_offset'] = 0;
    $count_function = $type['count'];
    $context['sandbox']['count'] = $count_function($recipient);
  }

  // Fetch the 10 next recipients.
  $load_function = $type['generate recipients'];
  $uids = $load_function($recipient, 10, $context['sandbox']['current_offset']);

  if (!empty($uids)) {
    foreach ($uids as $uid) {
      privatemsg_message_change_recipient($mid, $uid, 'hidden');
    }

    $context['sandbox']['current_offset'] += 10;
    // Set finished based on sandbox.
    $context['finished'] = empty($context['sandbox']['count']) ? 1 : ($context['sandbox']['current_offset'] / $context['sandbox']['count']);
  }
  else {
    // If no recipients were returned, mark as finished too.
    $context['sandbox']['finished'] = 1;
  }

  // If we are finished, mark the recipient as read.
  if ($context['finished'] >= 1) {
    db_update('pm_index')
      ->fields(array('is_new' => PRIVATEMSG_READ))
      ->condition('mid', $mid)
      ->condition('recipient', $recipient->recipient)
      ->condition('type', $recipient->type)
      ->execute();
  }
}