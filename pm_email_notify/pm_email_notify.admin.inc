<?php
// $Id$

/**
 * @file
 * Administration menu callbacks for pm_email_notify.module.
 */

/**
 * Menu callback for administration settings.
 */
function pm_email_notify_admin_settings_form() {
  $form['pm_email_notify_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify users of new private messages by default'),
    '#default_value' => variable_get('pm_email_notify_default', TRUE),
    '#weight' => 0,
  );

  $form['pm_email_notify_desc'] = array(
    '#type' => 'item',
    '#title' => t('Customize the email messages sent to users upon receipt of a new private message.'),
    '#weight' => 1,
  );

  $form['pm_email']['pm_email_notify_from'] = array(
    '#type' => 'textfield',
    '#title' => t('From e-mail address for notifications'),
    '#default_value' => variable_get('pm_email_notify_from',''),
    '#weight' => 2,
    '#description' => t('This is the e-mail address that notifications will come from. Leave blank to use the site default.'),
  );

  $form['pm_email_notify_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of notification messages'),
    '#default_value' => variable_get('pm_email_notify_subject', t('New private message at [site:name].')),
    '#weight' => 2,
  );

  $form['pm_email_notify_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body of notification messages'),
    '#default_value' => variable_get('pm_email_notify_body', _pm_email_notify_default_body()),
    '#weight' => 3,
  );

  if (module_exists('token')) {
    $form['token'] = array(
      '#type' => 'fieldset',
      '#title' => t('Token browser'),
      '#weight' => -1,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 4,
    );
    $form['token']['browser'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('privatemsg_message'),
    );
  }
  else {
    $form['tokens'] = array(
      '#type' => 'item',
      '#value' => t('Available variables are: !author, !author_uid, !pm_subject, !pm_body, !thread, !site, !login_url, !uri, !uri_brief, !message (URL) and !settings (URL).'),
      '#weight' => 4,
    );
  }

  return system_settings_form($form);
}