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
    $this->orderby = array();
  }
  
  function ensure_table() {
    
  }
  
  function add_field() {
    
  }
  
  function add_relationship() {
    
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
    
    foreach ($this->where as $group => $where) {
      $args = array_merge($args, $where['args']);
    }

    if ($this->having) {
      foreach ($this->having as $group => $having) {
        $args = array_merge($args, $having['args']);
      }
    }
    
    return $args;
  }
  
  function set_where_group($type = 'AND', $group = NULL, $where = 'where') {
    
  }
  
  function add_param($param, $value = '') {
    $this->params[$param] = $value;
  }
  
  function add_where($group, $clause) {
    
  }
  
  function add_orderby($table, $field, $order, $alias = '', $params = array()) {
    if ($table) {
      $this->ensure_table($table);
    }
    
    // Only fill out this aliasing if there is a table;
    // otherwise we assume it is a formula.
    if (!$alias && $table) {
      $as = $table . '_' . $field;
    }
    else {
      $as = $alias;
    }

    if ($field) {
      $as = $this->add_field($table, $field, $as, $params);
    }
    
    $this->orderby[] = array(
      'field' => $as,
      'direction' => strtoupper($order)
    );
  }
  
  function alter(&$view) {
    
  }
  
  function build(&$view) {
    $view->build_info['query'] = $this->query($view);
  }
  
  function apply_sort(&$data) {
    $args = array();
    if (is_array($this->orderby) && count($this->orderby)) {
      foreach ($this->orderby as $orderby) {
        $tmp = array(); dpm($data); dpm($orderby);
        foreach ($data as $key => $row) {
          $tmp[$key] = $row[$orderby['field']];
        }
        $args[] = $tmp;
        $args[] = constant('SORT_' . $orderby['direction']);
      }
      $args[] = &$data;
      
      call_user_func_array('array_multisort', $args);
    }
  }
  
  function fake_join(&$data, &$related_data) {
    foreach ($related_data as $column => $datum) {
      foreach ($data as $id => $row) {
        foreach ($datum['intersects'] as $intersect) {
          $data[$id]['client-' . $intersect] = $datum['data'][$data[$id]['client-id']][$intersect];
        }
      }
    }
  }
  
  function get_base_data() {
    return harvest_request(str_ireplace('harvest_api_', '', trim($this->base_table)));
  }
  
  function execute(&$view) {
    if (isset($view->build_info)) {
      $query = $view->build_info['query'];
    }
    
    dpm($view);
    
    if (1) { // if $query
      $start = $this->microtime();
      $data = $this->get_base_data();
      $related_data = array();      

      // Flatten certain results
      if ($this->base_table == 'harvest_api_projects') {
        $tmp = array();
        foreach ($data as $row) {
          foreach ($row as $cell) {
            $tmp[] = $cell;
          }
        }
        $data = $tmp;
      }

      // Add extra data
      if (is_array($data) && count($data)) {
        $first_row = current($data);
        if (isset($first_row['client-id'])) {
          $related_data['client-id']['data'] = harvest_request('clients');
          $first = current($data);
          $second = current($related_data['client-id']['data']); dpm($second);
          //$related_data['client-id']['intersects'] = array_intersect_key($first, $second);
          $related_data['client-id']['intersects'] = array_keys($second);
        }
      }
      
      // Fake join
      $this->fake_join($data, $related_data);
      
      // Sort
      $this->apply_sort($data);
      
      dpm($data, $this->base_table . ' table');
      
      foreach ($data as $key => $val) {
        $view->result[$key] = (object) $val;
        $this->$key = $val;
      }
      
      $view->total_rows = count($view->result);
      $view->execute_time = $this->microtime() - $start;
    }
    //dpm($view, '$view');
    //dpm($this, '$this');
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