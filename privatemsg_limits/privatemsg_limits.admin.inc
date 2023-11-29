<?php

/**
 * @file
 * Admin menu callbacks for privatemsg_limits module.
 */

/**
 * Menu callback for the admin configuration.
 */
function privatemsg_limits_admin($form, &$form_state) {
  $config = config('privatemsg_limits.settings');

  $form['conversation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Conversation settings'),
  );
  $form['conversation']['messages_per_thread'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of messages in a conversation'),
    '#description' => t('The maximum number of messages that can be included in a conversation. If this maximum has already been reached and a user attempts to add an additional message, then the action specified below shall be taken. Leave this set to 0 if you want unlimited messages per conversation.'),
    '#default_value' => $config->get('messages_per_thread'),
    '#size' => '3',
  );
  $form['conversation']['messages_per_thread_action'] = array(
    '#type' => 'radios',
    '#title' => t('Maximum messages action'),
    '#description' => t('This action shall be taken once a thread has already reached its maximum number of messages.'),
    '#default_value' => $config->get('messages_per_thread_action'),
    '#options' => array(
      'create-new' => t('Create new conversation'),
      'block-message' => t('Block message from being sent')
    ),
  );

  $form['receive'] = array(
    '#type' => 'fieldset',
    '#title' => t('Receive settings'),
  );
  $form['receive']['receive_enabled'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Limit the total number of messages/conversations that a user may have.'),
    '#default_value' => $config->get('receive_enabled'),
  );
  $form['receive']['receive_object'] = array(
    '#type'  => 'select',
    '#title' => t('What should be limited'),
    '#description' => t('Do you want to limit the number of individual messages a user can send, or limit the number of message conversations a user can initiate? An exceeded conversation limit will allow users to still write individual messages in existing conversations, but not start new ones.'),
    '#default_value' => $config->get('receive_object'),
    '#options' => array('message' => t('Messages'), 'thread' => t('Conversations')),
  );
  $form['receive']['receive_amount'] = array(
    '#type'  => 'number',
    '#title' => t('Maximum number of messages/conversations per user'),
    '#description' => t('The total number of messages/conversations that a user may have before they have to delete old messages/conversations. When this limit is reached, users are not allowed to receive or send new messages.'),
    '#default_value' => $config->get('receive_amount'),
    '#min' => 0,
    '#max' => 99999,
  );
  $form['receive'] += _privatemsg_limits_role_override_form('receive');

  $form['send'] = array(
    '#type' => 'fieldset',
    '#title' => t('Send settings'),
  );
  $form['send']['send_enabled'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Limit the total number of messages/conversations that a user can send in a given time period'),
    '#default_value' => $config->get('send_enabled'),
  );
  $form['send']['send_object'] = array(
    '#type'  => 'select',
    '#title' => t('What should be limited'),
    '#description' => t('Do you want to limit the number of individual messages a user can send, or limit the number of message conversations a user can initiate? An exceeded conversation limit will allow users to still write individual messages in existing conversations, but not start new ones.'),
    '#default_value' => $config->get('send_object'),
    '#options' => array('message' => t('Messages'), 'thread' => t('Conversations')),
  );
  $form['send']['send_amount'] = array(
    '#type'  => 'number',
    '#title' => t('Maximum number of messages/conversations per time period'),
    '#description' => t('The total number of messages/conversations that a user may send in a given time period.'),
    '#default_value' => $config->get('send_amount'),
    '#min' => 0,
    '#max' => 99999,
  );
  $form['send']['send_timeframe'] = array(
    '#type'  => 'select',
    '#title' => t('Set the time period to be used'),
    '#description' => t("Specify the time period to be used when limiting a user's sent messages/threads."),
    '#default_value' => $config->get('send_timeframe'),
    '#options' => backdrop_map_assoc(array(60, 300, 1800, 3600, 86400, 604800, 604800 * 4), 'format_interval'),
  );
  $form['send'] += _privatemsg_limits_role_override_form('send');

  $form['recipients'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recipient settings'),
  );
  $form['recipients']['recipients_enabled'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Limit the total number of recipients that a user can include in a single message'),
    '#default_value' => $config->get('recipients_enabled'),
  );
  $form['recipients']['recipients_amount'] = array(
    '#type'  => 'number',
    '#title' => t('Maximum number of recipients per message'),
    '#description' => t('Defines the total number of recipients a user may include in a single message. This setting only affects new messages (and not replies on existing message threads). A role (just like any other recipient type) is considered to be a single recipient regardless of the number of users with that role.'),
    '#default_value' => $config->get('recipients_amount'),
    '#min' => 0,
    '#max' => 99999,
  );
  $form['recipients'] += _privatemsg_limits_role_override_form('recipients');

  // Add a submit button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit handler for admin configuration.
 */
function privatemsg_limits_admin_submit($form, &$form_state) {
  $config = config('privatemsg_limits.settings');
  $values = $form_state['values'];
  $config->set('messages_per_thread', (int) $values['messages_per_thread']);
  $config->set('messages_per_thread_action', $values['messages_per_thread_action']);
  $config->set('receive_enabled', $values['receive_enabled']);
  $config->set('receive_object', $values['receive_object']);
  $config->set('receive_amount', (int) $values['receive_amount']);
  $config->set('send_enabled', $values['send_enabled']);
  $config->set('send_object', $values['send_object']);
  $config->set('send_amount', (int) $values['send_amount']);
  $config->set('send_timeframe', (int) $values['send_timeframe']);
  $config->set('messages_per_thread', (int) $values['messages_per_thread']);
  $config->set('recipients_enabled', $values['recipients_enabled']);
  $config->set('recipients_amount', (int) $values['recipients_amount']);
  foreach (array_keys(user_roles(TRUE)) as $role) {
    foreach (array('send', 'receive', 'recipients') as $option) {
      $setting = 'amount_per_role.' . $option . '.' . $role;
      $config->set($setting, $values["{$option}_amount_role_{$role}"]);
    }
  }
  $config->save();
  backdrop_set_message(t('The configuration options have been saved.'));
}

/**
 * Provides a generic rules form to override the default values.
 *
 * @param string $name    Unique name for the value
 *
 * @return array Form fieldset
 */
function _privatemsg_limits_role_override_form($name) {
  $config = config('privatemsg_limits.settings');
  $default = array(
    '#type'  => 'textfield',
    '#size' => 10,
  );
  $form['roles'] = array(
    '#type' => 'fieldset',
    '#description' => t('You may override the maximum limit specified above on a per-role basis. If a user has multiple roles, then the highest maximum value shall be used. Enter "unlimited" if a role may have unlimited messages/conversations. Enter "0" if you do not want to override the maximum limit setting.'),
    '#title'  => t('Override maximum limit by role'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  foreach (user_roles(TRUE) as $id => $role) {
    $form['roles']["{$name}_amount_role_{$id}"] = $default + array(
      '#title' => $role,
      '#default_value' => $config->get("amount_per_role.{$name}.{$id}"),
    );
  }
  return $form;
}
