<?php

/**
 * Implements hook_views_data().
 */
function harvest_views_data() {
  $data['harvest_api']['table']['group']  = t('Harvest_API');
  
  // Advertise this table as a possible base table
  $data['harvest_api']['table']['base'] = array(
    'field' => 'test',
    'title' => t('Harvest_API'),
    'help' => t('Directly query Harvest to get live time report data.'),
    'query class' => 'harvest_query',
  );
  
  $data['harvest_api']['email'] = array(
    'title' => t('Email'),
    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_field_email',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function harvest_views_plugins() {
  return array(
    'query' => array(
      'harvest_query' => array(
        'title' => t('Harvest Query'),
        'help' => t('Query will be generated and run using Harvest API.'),
        'handler' => 'harvest_plugin_query_harvest',
      ),
    ),
  );
}