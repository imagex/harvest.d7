<?php

class harvest_plugin_query_harvest extends views_plugin_query {
  
  /**
   * Constructor; Create the basic query object and fill with default values.
   */
  function init($base_table = 'harvest_api', $base_field, $options = array()) {
    module_load_include('inc', 'harvest');
    parent::init($base_table, $base_field, $options);
    
    $this->params = '';
    $this->start = '';
  }
  
  function condition_query($where = 'where') {
    
    return '';
  }
  
  function user_pager() {
    return FALSE;
  }
  
  /**
   * Generate a query and a countquery from all of the information supplied
   */
  function query($view, $get_count = FALSE) {
    $where = $this->condition_query();
    $params = $this->params;

    return $params;
  }
  
  function get_where_args() {
    $args = array();
    
    return $args;
  }
  
  function set_where_group($type = 'AND', $group = NULL, $where = 'where') {
    
  }
  
  function add_param($param, $value = '') {
    $this->params[$param] = $value;
  }
  
  function add_where($group, $clause) {
    
  }
  
  function alter(&$view) {
    
  }
  
  function build(&$view) {
    $view->build_info['query'] = $this->query($view);
  }
  
  function execute(&$view) { dpm('execute');
    $query = $view->build_info['query']; dpm($query, '$query');
    
    if (1) { // if $query
      $start = $this->microtime();
      $people_raw = harvest_request('people');
      
      foreach ($people_raw as $key => $val) {
        $view->result[$key] = (object) $val;
        $this->$key = $val;
      }
      
      $view->total_rows = count($view->result);
      $view->execute_time = $this->microtime() - $start;
    }
    dpm($view, '$view');
    dpm($this, '$this');
  }
  
  function add_signature(&$view) {
    
  }
  
  function option_definition() {
    $options = parent::option_definition();
    
    return $options;
  }
  
  function options_validate(&$form, &$form_state) {
    
  }
  
  function options_submit(&$form, &$form_state) {
    
  }
  
  function microtime() {
    list($usec, $sec) = explode(' ', microtime());
    return (float)$sec + (float)$usec;
  }
}