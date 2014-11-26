<?php
// $Id$
/**
 * @file
 * Administration functions for the harvest API.
 */

/**
 * Menu callback; Form for harvest API settings.
 */
function harvest_settings_form() {
  drupal_add_css(drupal_get_path('module', 'harvest') . '/css/styles.css', array('media' => 'screen'));
  // General module settings.
  $form['cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache settings'),
    '#collapsible' => TRUE,
  );
  $form['cache']['cache_life'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache life'),
    '#description' => t('Enter the number of seconds until a cached request should expire. This setting can help throttle the number of requests to the harvest API.'),
    '#size' => 30,
    '#required' => TRUE,
    '#default_value' => variable_get('harvest_cache_life', '3600'),
  );

  // Account specific settings; includes URL, email, and password.
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account settings'),
    '#collapsible' => TRUE,
  );
  $form['account']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('Enter the URL in which you can access your harvest account (ex: https://accountname.harvestapp.com).'),
    '#required' => TRUE,
    '#default_value' => variable_get('harvest_url', ''),
  );
  $form['account']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#description' => t('Enter the email of the account we will login to during an API request. It is recommended that a separate administrator account is setup.'),
    '#required' => TRUE,
    '#default_value' => variable_get('harvest_email', ''),
    '#prefix' => variable_get('harvest_login', '') ? t('<div class="warning messages">To change the email you must enter a password.</div>') : '',
  );
  $form['account']['pass'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('This password will be encoded using base64, the original value will not be stored for security reasons.'),
    '#required' => variable_get('harvest_login', '') ? FALSE : TRUE,
    '#prefix' => variable_get('harvest_login', '') ? t('<div class="warning messages">Password has been saved, you can specify a new one below.</div>') : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
  );

  return $form;
}

/**
 * Validates the harvest API settings; peforming a test request to the API.
 */
function harvest_settings_form_validate($form, $form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-submit') {
    // Cache life must be a positive number.
    if (!is_numeric($form_state['values']['cache_life']) || $form_state['values']['cache_life'] < 1) {
      form_set_error('cache_life', t('Cache life must be a positive number.'));
    }

    // Check if a password is defined when changing an existing email.
    if (drupal_strlen(variable_get('harvest_email', '')) && variable_get('harvest_email', '') != trim($form_state['values']['email']) && !drupal_strlen(trim($form_state['values']['pass']))) {
      form_set_error('pass', t('Password is required to change the email address.'));
    }

    // Test the settings with an actual request to the API.
    if (!form_get_errors()) {
      $login = drupal_strlen(trim($form_state['values']['pass'])) ? base64_encode(trim($form_state['values']['email']) . ':' . trim($form_state['values']['pass'])) : variable_get('harvest_login', '');
      if (($req = harvest_request('people', array('return' => 'code', 'url' => $form_state['values']['url'], 'login' => $login), TRUE)) != '200') {
        form_set_error('', t('An error occurred accessing the harvest API, please double check your account settings.'));
      }
    }
  }
}

/**
 * Processes the harvest API settings form, combing the email and password using base64.
 */
function harvest_settings_form_submit($form, $form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-submit') {
    variable_set('harvest_cache_life', $form_state['values']['cache_life']);
    variable_set('harvest_url', trim($form_state['values']['url']));
    if (variable_get('harvest_email', '') != trim($form_state['values']['email']) || (drupal_strlen(variable_get('harvest_login', '')) && drupal_strlen(trim($form_state['values']['pass'])))) {
      variable_set('harvest_email', trim($form_state['values']['email']));
      variable_set('harvest_login', base64_encode(trim($form_state['values']['email']) . ':' . trim($form_state['values']['pass'])));
    }
    drupal_set_message(t('Your changes have been successfully saved.'));
  }
  else {
    variable_del('harvest_cache_life');
    variable_del('harvest_url');
    variable_del('harvest_email');
    variable_del('harvest_login');
    drupal_set_message(t('Settings have been reset to there defaults.'));
  }
}