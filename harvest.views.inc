<?php

/**
 * Implements hook_views_data().
 */
function harvest_views_data() {
  $data['harvest_api_people']['table']['group']  = t('Harvest API People');
  
  // Advertise this table as a possible base table
  $data['harvest_api_people']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Harvest API People'),
    'help' => t('Directly query Harvest to get live time report data.'),
    'query class' => 'harvest_query_people',
  );
  $data['harvest_api_people']['id'] = array(
    'title' => t('ID'),
//    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_people_field_id',
      'click sortable' => TRUE,
    ),
  );
  $data['harvest_api_people']['email'] = array(
    'title' => t('Email'),
    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_people_field_email',
      'click sortable' => TRUE,
    ),
  );
  $data['harvest_api_people']['first-name'] = array(
    'title' => t('First Name'),
//    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_people_field_first_name',
      'click sortable' => TRUE,
    ),
  );
  $data['harvest_api_people']['last-name'] = array(
    'title' => t('Last Name'),
//    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_people_field_last_name',
      'click sortable' => TRUE,
    ),
  );
  
  $data['harvest_api_tasks']['table']['group']  = t('Harvest API Tasks');
  
  // Advertise this table as a possible base table
  $data['harvest_api_tasks']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Harvest API Tasks'),
    'help' => t('Directly query Harvest to get live time report data.'),
    'query class' => 'harvest_query_tasks',
  );

  $data['harvest_api_tasks']['id'] = array(
    'title' => t('ID'),
    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_tasks_field_id',
      'click sortable' => TRUE,
    ),
  );
  $data['harvest_api_tasks']['name'] = array(
    'title' => t('Name'),
    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_tasks_field_name',
      'click sortable' => TRUE,
    ),
  );
  
  $data['harvest_api_clients']['table']['group'] = t('Harvest API Clients');
  $data['harvest_api_clients']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Harvest API Clients'),
    'help' => t('Directly query Harvest to get live time report data.'),
    'query class' => 'harvest_query_clients',
  );
  $data['harvest_api_clients']['client-id-id'] = array(
    'title' => t('ID'),
    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_clients_field_id',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'harvest_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'harvest_handler_projects_field_client_id',
      'base' => 'harvest_api_projects',
      'field' => 'client-id',
      'label' => t('Project'),
    ),
  );
  $data['harvest_api_clients']['client-name'] = array(
    'title' => t('Name'),
    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_clients_field_name',
      'click sortable' => TRUE,
    ),
  );
  
  $data['harvest_api_projects']['table']['group']  = t('Harvest API Projects');
  
  // Advertise this table as a possible base table
  $data['harvest_api_projects']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Harvest API Projects'),
    'help' => t('Directly query Harvest to get live time report data.'),
    'query class' => 'harvest_query_projects',
  );

  $data['harvest_api_projects']['client-id'] = array(
    'title' => t('Client ID'),
    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_projects_field_client_id',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'handler' => 'harvest_handler_relationship',
      'base' => 'harvest_api_clients',
      'field' => 'client-id',
      'label' => t('Client'),
    ),
  );
  $data['harvest_api_projects']['id'] = array(
    'title' => t('ID'),
    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_projects_field_id',
      'click sortable' => TRUE,
    ),
  );
  $data['harvest_api_projects']['name'] = array(
    'title' => t('Name'),
    'help' => t('A test field for debugging purposes'),
    'field' => array(
      'handler' => 'harvest_handler_projects_field_name',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'harvest_handler_sort',
    ),
  );
  
  $data['harvest_api_entries']['table']['group']  = t('Harvest API Time Entries');

  // Advertise this table as a possible base table
  $data['harvest_api_entries']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Harvest API Time Entries'),
    'help' => t('Time entries.'),
    'query class' => 'harvest_query_entries',
  );
  
  $data['harvest_api_statistics']['table']['group']  = t('Harvest API Statistics');
  
  $data['harvest_api_statistics']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Harvest API Statistics'),
    'help' => t('Statistics reports.'),
    'query class' => 'harvest_query_statistics',
  );
  
  $data['harvest_api_statistics']['budget'] = array(
    'title' => t('Budget'),
    'field' => array(
      'handler' => 'harvest_handler_statistics_field_budget',
    ),
  );
  
  $data['harvest_api_statistics']['spent'] = array(
    'title' => t('Spent'),
    'field' => array(
      'handler' => 'harvest_handler_statistics_field_spent',
    ),
    'filter' => array(
      'handler' => 'harvest_handler_filter_numeric',
    ),
  );
  
  $data['harvest_api_statistics']['task-name'] = array(
    'title' => t('Task'),
    'field' => array(
      'handler' => 'harvest_handler_statistics_field_task_name',
    ),
  );
  
  $data['harvest_api_statistics']['project_id'] = array(
    'title' => t('Project ID'),
    'help' => t('Unique ID for the project.'),
    'argument' => array(
      'handler' => 'harvest_handler_argument_project_id',
      'numeric' => TRUE,
    ),
  );
  
  $data['harvest_api_statistics']['statistics-spent-at'] = array(
    'title' => t('Spent at'),
    'filter' => array(
      'handler' => 'harvest_handler_filter_spent_at_date'
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
      'harvest_query_people' => array(
        'title' => t('Harvest API People'),
        'help' => t('Query will be generated and run using Harvest API.'),
        'handler' => 'harvest_plugin_query_harvest_people',
      ),
      'harvest_query_projects' => array(
        'title' => t('Harvest API Projects'),
        'help' => t('Query will be generated and run using Harvest API.'),
        'handler' => 'harvest_plugin_query_harvest_projects',
      ),
      'harvest_query_tasks' => array(
        'title' => t('Harvest API Tasks'),
        'help' => t('Query will be generated and run using Harvest API.'),
        'handler' => 'harvest_plugin_query_harvest_tasks',
      ),
      'harvest_query_clients' => array(
        'title' => t('Harvest API Clients'),
        'help' => t('Query will be generated and run using Harvest API.'),
        'handler' => 'harvest_plugin_query_harvest_clients',
      ),
      'harvest_query_entries' => array(
        'title' => t('Harvest API Time Entries'),
        'help' => t('Query will be generated and run using Harvest API.'),
        'handler' => 'harvest_plugin_query_harvest_entries',
      ),
      'harvest_query_statistics' => array(
        'title' => t('Harvest API Statistics'),
        'help' => t('Query will be generated and run using Harvest API.'),
        'handler' => 'harvest_plugin_query_harvest_statistics',
      ),
    ),
  );
}