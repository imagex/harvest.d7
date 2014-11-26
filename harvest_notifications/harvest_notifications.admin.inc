<?php
// $Id$
/**
 * @file
 * Administration functions for harvest notifications.
 */

/**
 * Menu callback; form for notification settings.
 */
function harvest_notifications_settings_form() {
  $roles = user_roles(TRUE);

  // Testing/debugging
  $form['harvest_notifications_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debug/test mode'),
    '#description' => t('When enabled all emails will be logged instead of being sent out.'),
    '#default_value' => variable_get('harvest_notifications_debug', 0),
  );

  // Daily
  $form['daily'] = array(
    '#type' => 'fieldset',
    '#title' => t('Daily notifications'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['daily']['harvest_notifications_daily'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable daily notifications?'),
    '#description' => t('Check the box above to enable sending of daily notifications.'),
    '#default_value' => harvest_notifications_variable('harvest_notifications_daily'),
  );
  $form['daily']['harvest_notifications_daily_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('Enter the subject of the daily notification sent to users.'),
    '#default_value' => harvest_notifications_variable('harvest_notifications_daily_subject'),
    '#required' => TRUE,
  );
  $form['daily']['harvest_notifications_daily_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#description' => t('Enter the body of the daily notification sent to users.'),
    '#default_value' => harvest_notifications_variable('harvest_notifications_daily_body'),
    '#required' => TRUE,
  );
  $form['daily']['harvest_notifications_daily_from'] = array(
    '#type' => 'textfield',
    '#title' => t('From e-mail address'),
    '#description' => t('Optionally specify a from e-mail, the default will be the site e-mail (!sitemail).', array('!sitemail' => variable_get('site_mail', ''))),
    '#default_value' => harvest_notifications_variable('harvest_notifications_daily_from'),
  );

  // Weekly
  $form['weekly'] = array(
    '#type' => 'fieldset',
    '#title' => t('Weekly notificiations'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  if (module_exists('content_access')) {
    $form['weekly']['harvest_notifications_weekly'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable weekly notifications?'),
      '#description' => t('Check the box above to enable sending of weekly notifications.'),
      '#default_value' => harvest_notifications_variable('harvest_notifications_weekly'),
    );
    $form['weekly']['harvest_notifications_weekly_sendon'] = array(
      '#type' => 'select',
      '#title' => t('Send on day'),
      '#description' => t('Select the day in which to send weekly notifications on.'),
      '#options' => array(
        'Monday' => t('Monday'),
        'Tuesday' => t('Tuesday'),
        'Wednesday' => t('Wednesday'),
        'Thursday' => t('Thursday'),
        'Friday' => t('Friday'),
        'Saturday' => t('Saturday'),
        'Sunday' => t('Sunday'),
      ),
      '#default_value' => harvest_notifications_variable('harvest_notifications_weekly_sendon'),
      '#required' => TRUE,
    );
    $form['weekly']['harvest_notifications_weekly_subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#description' => t('Enter the subject of the weekly notification sent to users.'),
      '#default_value' => harvest_notifications_variable('harvest_notifications_weekly_subject'),
      '#required' => TRUE,
    );
    $form['weekly']['harvest_notifications_weekly_body'] = array(
      '#type' => 'textarea',
      '#title' => t('Body'),
      '#description' => t('Enter the body of the weekly notification sent to users.'),
      '#default_value' => harvest_notifications_variable('harvest_notifications_weekly_body'),
      '#required' => TRUE,
    );
    $form['weekly']['harvest_notifications_weekly_from'] = array(
      '#type' => 'textfield',
      '#title' => t('From e-mail address'),
      '#description' => t('Optionally specify a from e-mail, the default will be the site e-mail (!sitemail).', array('!sitemail' => variable_get('site_mail', ''))),
      '#default_value' => harvest_notifications_variable('harvest_notifications_weekly_from'),
    );
  }
  else {
    $form['weekly']['info'] = array(
      '#value' => t('Weekly notifications is dependent on the !project module.', array(
        '!project' => l(t('content access'), 'http://drupal.org/project/content_access', array('absolute' => TRUE))
      )),
      '#prefix' => '<div class="warning">',
      '#suffix' => '</div>',
    );
  }

  // Reports
  $form['report'] = array(
    '#type' => 'fieldset',
    '#title' => t('Report notifications'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['report']['harvest_notifications_report'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable report notifications?'),
    '#description' => t('Check the box above to enable sending of report notifications.'),
    '#default_value' => harvest_notifications_variable('harvest_notifications_report'),
  );
  $form['report']['harvest_notifications_report_roles'] = array(
    '#type' => 'select',
    '#title' => t('Roles'),
    '#description' => t('Select one or more roles that can recieve this notification type.'),
    '#options' => $roles,
    '#multiple' => TRUE,
    '#default_value' => harvest_notifications_variable('harvest_notifications_report_roles'),
  );
  $form['report']['harvest_notifications_report_startdate'] = array(
    '#type' => 'textfield',
    '#title' => t('Start sending on'),
    '#description' => t('Enter a date to start sending the reports on in the following format: MM-DD-YYYY'),
    '#required' => TRUE,
    '#default_value' => harvest_notifications_variable('harvest_notifications_report_startdate'),
  );
  $form['report']['harvest_notifications_report_sendon'] = array(
    '#type' => 'select',
    '#title' => t('Send on day'),
    '#description' => t('Select the day in which to send report notifications on.'),
    '#options' => array(
      'Monday' => t('Monday'),
      'Tuesday' => t('Tuesday'),
      'Wednesday' => t('Wednesday'),
      'Thursday' => t('Thursday'),
      'Friday' => t('Friday'),
      'Saturday' => t('Saturday'),
      'Sunday' => t('Sunday'),
    ),
    '#default_value' => harvest_notifications_variable('harvest_notifications_report_sendon'),
    '#required' => TRUE,
  );
  $form['report']['harvest_notifications_report_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Weekly sending interval'),
    '#description' => t('Enter the number of weeks between each notification sending.'),
    '#size' => 15,
    '#default_value' => harvest_notifications_variable('harvest_notifications_report_interval'),
    '#required' => TRUE,
  );
  $form['report']['harvest_notifications_report_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('Enter the subject of the report notification sent to users.'),
    '#default_value' => harvest_notifications_variable('harvest_notifications_report_subject'),
    '#required' => TRUE,
  );
  $form['report']['harvest_notifications_report_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#description' => t('Enter the body of the report notification sent to users.'),
    '#default_value' => harvest_notifications_variable('harvest_notifications_report_body'),
    '#required' => TRUE,
  );
  $form['report']['harvest_notifications_report_from'] = array(
    '#type' => 'textfield',
    '#title' => t('From e-mail address'),
    '#description' => t('Optionally specify a from e-mail, the default will be the site e-mail (!sitemail).', array('!sitemail' => variable_get('site_mail', ''))),
    '#default_value' => harvest_notifications_variable('harvest_notifications_report_from'),
  );

  // Tokens
  
  $tokens = token_info();
  
  $form['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notification replacements'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#value' => theme('token_tree', array('token_types' => 'all')),
  );

  return system_settings_form($form);
}

/**
 * Menu callback; form for user timezones.
 */
function harvest_notifications_timezones_form() {
  $people = harvest_request('people');

  // Run through each person and create new instance.
  $person_ids = array();
  $saved_timezones = variable_get('harvest_notifications_timezones', array());
  if (is_array($people) && count($people)) {
    // Prepare timezone value options.
    $timezones = array_flip(timezone_identifiers_list());
    foreach ($timezones as $timezone => $value) {
      $timezones[$timezone] = $timezone;
    }

    // Option to set default site timezone (separate from Drupal).
    $form['default'] = array(
      '#type' => 'select',
      '#title' => t('Default timezone'),
      '#description' => t('NOTE: This setting is separate from the default Drupal timezone settings.'),
      '#options' => $timezones,
      '#default_value' => variable_get('harvest_notifications_default_timezone', ''),
    );

    // Add site default option to timezones list.
    $timezones = array_merge(array('Default' => 'Use Site Default'), $timezones);

    // Create fields for each person and there timezone.
    foreach ($people as $person) {
      $person_ids[] = $person['id'];

      $form["person_{$person['id']}"] = array(
        '#tree' => TRUE,
      );
      $form["person_{$person['id']}"]['name'] = array(
        '#markup' => $person['first-name'] . ' ' . $person['last-name'],
      );
      $form["person_{$person['id']}"]['timezone'] = array(
        '#type' => 'select',
        '#options' => $timezones,
        '#default_value' => isset($saved_timezones[$person['id']]) ? $saved_timezones[$person['id']] : '',
      );
    }

    $form['people'] = array(
      '#type' => 'hidden',
      '#value' => serialize($person_ids),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Changes'),
    );
  }

  // Throw error when list of people could not be retrieved.
  else {
    $form['notice'] = array(
      '#value' => t('Could not load list of harvest people, try again in 15 minutes.'),
    );
  }

  return $form;
}

/**
 * Saves timezone mapping to people.
 */
function harvest_notifications_timezones_form_submit($form, $form_state) {
  variable_set('harvest_notifications_default_timezone', $form_state['values']['default']);
  $timezones = array();
  $people = unserialize($form_state['values']['people']);
  foreach ($people as $person_id) {
    $timezones[$person_id] = $form_state['values']["person_{$person_id}"]['timezone'];
  }
  variable_set('harvest_notifications_timezones', $timezones);
}

/**
 * Themes the timezone configuration form.
 */
function theme_harvest_notifications_timezones_form($variables) {
  $form = &$variables['form'];

  // Header
  $header = array(t('Person'), t('Timezone'));

  // Rows
  $people = unserialize($form['people']['#value']);
  foreach ($people as $person_id) {
    $rows[] = array(
      render($form["person_$person_id"]['name']),
      render($form["person_$person_id"]['timezone']),
    );
  }

  // Ouput
  $output = '';
  $output .= render($form['notice']);
  $output .= render($form['default']);
  if (count($rows)) {
    $output .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array(), 'caption' => '', 'empty' => t('None'), 'sticky' => FALSE, 'colgroups' => array()));
  }
  $output .= drupal_render_children($form);

  return $output;
}