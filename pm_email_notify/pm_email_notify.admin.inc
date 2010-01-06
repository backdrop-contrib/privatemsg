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
  $form['pm_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Privatemsg e-mail notification'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['pm_email']['pm_email_notify_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify users of new private messages by default'),
    '#default_value' => variable_get('pm_email_notify_default', TRUE),
    '#weight' => 0,
  );

  $form['pm_email']['pm_email_notify_desc'] = array(
    '#type' => 'item',
    '#value' => t('Customize the email messages sent to users upon receipt of a new private message. <br />Available variables are: !author, !author_uid, !pm_subject, !pm_body, !thread, !site, !login_url, !uri, !uri_brief, !message (URL) and !settings (URL).'),
    '#weight' => 1,
  );

  $form['pm_email']['pm_email_notify_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of notification messages'),
    '#default_value' => variable_get('pm_email_notify_subject', 'New private message at !site.'),
    '#weight' => 2,
  );

  $form['pm_email']['pm_email_notify_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body of notification messages'),
    '#default_value' => variable_get('pm_email_notify_body', _pm_email_notify_default_body()),
    '#weight' => 3,
  );
  return system_settings_form($form);
}