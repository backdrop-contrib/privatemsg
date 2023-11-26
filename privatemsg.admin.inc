<?php

/**
 * @file
 * Administration menu callbacks for Privatemsg.
 */

function privatemsg_admin_settings() {
  $form = array();
  $config = config('privatemsg.settings');

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['display_settings'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#title'       => t('Display'),
    '#weight'      => 5,
    '#group'       => 'settings',
  );
  $form['display_settings']['private_message_view_template'] = array(
    '#type'          => 'radios',
    '#title'         => t('Private message display template'),
    '#default_value' => $config->get('private_message_view_template'),
    '#options'       => privatemsg_view_options(),
    '#weight'        => -10,
  );
  $form['display_settings']['display_loginmessage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inform the user about new messages on login'),
    '#default_value' => $config->get('display_loginmessage'),
    '#description' => t('This option can safely be disabled if the "New message indication" block is used instead.'),
    '#weight'      => -5,
  );
  $form['display_settings']['display_disabled_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inform the user on /messages pages that he can not write new messages when privatemsg is disabled.'),
    '#default_value' => $config->get('display_disabled_message'),
    '#description' => t('Users can (if given the permission) disable Privatemsg which disallows writing messages to them and they can not write messages themself. If enabled, those users are informed on the relevant pages why they are not allowed to write messages.'),
    '#weight'      => -8,
  );
  $form['display_settings']['default_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Default redirection URL'),
    '#default_value' => $config->get('default_redirect'),
    '#description' => t('Defines to which page users are sent after sending a new message. &lt;front&gt; can be used for the defined front page and &lt;new-message&gt; to redirect to the new message. Leave empty to disable the redirect.'),
    '#weight'      => 0,
  );
  $form['display_settings']['default_redirect_reply'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect replies too'),
    '#default_value' => $config->get('default_redirect_reply'),
    '#description' => t('Also redirect to the defined page when replying.'),
    '#weight'      => 1,
  );
  $form['display_settings']['display_preview_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show preview button on compose form'),
    '#description' => t('If checked, displays a preview button when sending new messages.'),
    '#default_value' => $config->get('display_preview_button'),
    '#weight' => 5,
  );

  $form['flush_deleted'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#title'       => t('Deleted messages'),
    '#description' => t('By default, deleted messages are only hidden from the user but still stored in the database. These settings control if and when messages should be removed.'),
    '#weight'    => 20,
    '#group'       => 'settings',
  );

  $form['flush_deleted']['flush_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Flush deleted messages'),
    '#default_value' => $config->get('flush_enabled'),
    '#description'   => t('Enable the flushing of deleted messages. Requires that cron is enabled'),
  );

  $form['flush_deleted']['flush_days'] = array(
    '#type' => 'select',
    '#title' => t('Flush messages after they have been deleted for more days than'),
    '#default_value' => $config->get('flush_days'),
    '#options' => backdrop_map_assoc(array(0, 1, 2, 5, 10, 30, 100)),
  );

  $form['flush_deleted']['flush_max'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of messages to flush per cron run'),
    '#default_value' => $config->get('flush_max'),
    '#options' => backdrop_map_assoc(array(50, 100, 200, 500, 1000)),
  );

  $form['privatemsg_listing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message lists'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -5,
    '#group'       => 'settings',
  );

  $form['privatemsg_listing']['per_page'] = array(
    '#type' => 'select',
    '#title' => t('Threads per page'),
    '#default_value' => $config->get('per_page'),
    '#options' => backdrop_map_assoc(array(10, 25, 50, 75, 100)),
    '#description' => t('Choose the number of conversations that should be listed per page.'),
  );

  $form['privatemsg_listing']['display_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Configure fields'),
    '#description' => t('Select which columns/fields should be displayed in the message listings. Subject and Last updated cannot be disabled.'),
    '#options' => array(
      'participants'    => t('Participants'),
      'thread_started'  => t('Started'),
      'count'           => t('Messages'),
    ),
    '#default_value' => $config->get('display_fields'),
  );

  $amounts = backdrop_map_assoc(array(5, 10, 20, 30, 50, 70, 90, 150, 200, 250, 300));
  $form['privatemsg_listing']['view_max_amount'] = array(
    '#type'          => 'select',
    '#title'         => t('Number of messages on thread pages'),
    '#options'       => $amounts + array(PRIVATEMSG_UNLIMITED => t('Unlimited')),
    '#default_value' => $config->get('view_max_amount'),
    '#description'   => t('Threads will not show more than this number of messages on a single page.'),
    '#weight'        => 10,
  );

  $form['privatemsg_listing']['view_use_max_as_default'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display different amount of messages on first thread page'),
    '#default_value' => $config->get('view_use_max_as_default'),
    '#description'   => t('By default, the first thread page shows the maximally allowed amount of messages. Enable this checkbox to set a different value.'),
    '#weight'        => 15,
  );

  $form['privatemsg_listing']['view_default_amount'] = array(
    '#prefix'        => '<div id="privatemsg-view-default-button">',
    '#suffix'        => '</div>',
    '#type'          => 'select',
    '#title'         => t('Number of messages on first thread page'),
    '#default_value' => $config->get('view_default_amount'),
    '#description'   => t('The number of messages to be displayed on first thread page. Displays the newest messages.'),
    '#options'       => $amounts,
    '#weight'        => 20,
    '#states'        => array(
      'visible' => array(
        "input[name='privatemsg_view_use_max_as_default']" => array('checked' => TRUE),
      )
    )
  );

  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 10,
    '#group'       => 'settings',
  );

  $form['links']['display_link_self'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display "Send this user a message" links for themself'),
    '#description' => t('If enabled, each users sees that link on their own profile, comments and similiar places.'),
    '#default_value' => $config->get('display_link_self'),
    '#weight'        => -10,
  );

  $form['links']['display_profile_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display link on profile pages.'),
    '#description' => t('If this setting is enabled, a link to send a private message will be displayed.'),
    '#default_value' => $config->get('display_profile_links'),
  );

  $node_types = node_type_get_names();
  $form['links']['link_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display link on the selected content types'),
    '#description' => t('Select which content types should display a link to send a private message to the author. By default, the link is not displayed below teasers.'),
    '#default_value' => $config->get('link_node_types'),
    '#options' => $node_types,
  );
  $form['links']['display_on_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display link on teasers of the selected content types.'),
    '#default_value' => $config->get('display_on_teaser'),
  );
  $form['links']['display_on_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display links on comments of selected content types.'),
    '#description' => t('Also display a link to send a private message to the authors of the comments of the selected content types.'),
    '#default_value' => $config->get('display_on_comments'),
  );

  $form['#submit'][] = 'privatemsg_admin_settings_submit';


  // Add a submit button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}


/**
 * Submit handler for admin settings form.
 */
function privatemsg_admin_settings_submit($form, &$form_state) {
  backdrop_theme_rebuild();
  $config = config('privatemsg.settings');
  $config->set('private_message_view_template',$form_state['values']['private_message_view_template']);
  $config->set('display_loginmessage',$form_state['values']['display_loginmessage']);
  $config->set('display_disabled_message',$form_state['values']['display_disabled_message']);
  $config->set('default_redirect',$form_state['values']['default_redirect']);
  $config->set('default_redirect_reply',$form_state['values']['default_redirect_reply']);
  $config->set('display_preview_button',$form_state['values']['display_preview_button']);
  $config->set('flush_enabled',$form_state['values']['flush_enabled']);
  $config->set('flush_days',$form_state['values']['flush_days']);
  $config->set('flush_max',$form_state['values']['flush_max']);
  $config->set('per_page',$form_state['values']['per_page']);
  $config->set('display_fields',$form_state['values']['display_fields']);
  $config->set('view_max_amount',$form_state['values']['view_max_amount']);
  $config->set('view_use_max_as_default',$form_state['values']['view_use_max_as_default']);
  $config->set('view_default_amount',$form_state['values']['view_default_amount']);
  $config->set('display_link_self',$form_state['values']['display_link_self']);
  $config->set('display_profile_links',$form_state['values']['display_profile_links']);
  $config->set('link_node_types',$form_state['values']['link_node_types']);
  $config->set('display_on_teaser',$form_state['values']['display_on_teaser']);
  $config->set('display_on_comments',$form_state['values']['display_on_comments']);
  $config->save();
  backdrop_set_message(t('The configuration options have been saved.'));
}


function privatemsg_view_options() {
  $options = module_invoke_all('privatemsg_view_template');
  return $options;
}
