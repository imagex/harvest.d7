<?php

class harvest_plugin_query_harvest_statistics extends harvest_plugin_query_harvest {
  function add_where($group, $clause) {
    $args = func_get_args();
    if (count($args) == 2) {
      $this->where[$group]['clauses'][] = $clause;
    }
    elseif (count($args) == 4) {
      $col = $clause;
      $val = $args[2];
      $op = $args[3];
      switch ($col) {
        case '.spent':
          $this->where_php[$group]['clauses'][] = array('col' => trim($col, '.'), 'val' => $val, 'op' => '>');
          break;
      }
    }
  }
  
  function execute(&$view) {
    $data = array();
    $start = $this->microtime();
    
    if (is_array($view->argument) && isset($view->argument['project_id'])) {
      $argument_project_id = $view->argument['project_id'];
      $tasks_data = harvest_projects_get_tasks($argument_project_id->value[0]);
      
      $queries = $this->array_flatten($this->where);
      $query = implode('&', $queries);
      
      $statistics_data = harvest_projects_get_statistics($argument_project_id->value[0], array('query' => $query));
      
      // Apply filters with PHP
      $this->where_php($statistics_data);

      foreach ($statistics_data['categories'] as $category_id => $category) {
        $data[] = (object) array_merge($category['category'], array('category_id' => $category_id, 'task_id' => 0, 'task-name' => '<strong>' . $tasks_data[$category_id]['category']->name . '</strong>'));
        foreach ($category['tasks'] as $task_id => $task) {
          $data[] = (object) array_merge($task, array('category_id' => 0, 'task_id' => $task_id, 'task-name' => $tasks_data[$category_id]['tasks'][$task_id]['name']));
        }
      }
      $data[] = (object) array_merge($statistics_data['overall'], array('category_id' => 0, 'task_id' => 0, 'task-name' => t('Totals (Billable Only):')));
    }
    
    $view->result = $data;
    $view->total_rows = count($view->result);
    $view->execute_time = $this->microtime() - $start;
  }
  
  function where_php(&$data, $filters = array()) {
    $filters = $this->where_php;
    $adjustments = array();
    
    // Go through the data
    foreach ($data['categories'] as $category_id => $category) {
      foreach ($category['tasks'] as $task_id => $task) {
        $local_adjustments = array();
        foreach ($filters as $filter) {
          foreach ($filter['clauses'] as $clause) {
            $eval = '((float)' . $task[$clause['col']] . ')' . $clause['op'] . $clause['val'];
            if (!eval('return ' . $eval . ';')) {
              unset($data['categories'][$category_id]['tasks'][$task_id]);
              $local_adjustments[$clause['col']] = (float)$task[$clause['col']];
              
            }
          }
        }

        // Adjust the category row
        foreach ($local_adjustments as $local_adjustment_key => $local_adjustment) {
          $data['categories'][$category_id]['category'][$local_adjustment_key] = $data['categories'][$category_id]['category'][$local_adjustment_key] - $local_adjustment;
        }
      }
      
      // If category row is zero, remove it
      $eval = '((float)' . $data['categories'][$category_id]['category'][$clause['col']] . ')' . $clause['op'] . $clause['val'];
      if (!eval('return ' . $eval . ';')) {
        unset($data['categories'][$category_id]);
      }
    }
  }
  
  /**
   * Note that in a multidimensional array, each element may be identified by a _sequence_ of keys, i.e. the keys that lead towards that element. Thus "preserving keys" may have different interpretations. Ivan's function for example creates a two-dimensional array preserving the last two keys. Other functions below create a one-dimensional array preserving the last key. For completeness, I will add a function that merges the key sequence by a given separator and a function that preserves the last n keys, where n is arbitrary.
   * same array_flatten function, compressed and preserving keys.
   */
  function array_flatten($a,$f=array()){
    if(!$a||!is_array($a))return array();
    foreach($a as $k=>$v){
      if(is_array($v))$f=$this->array_flatten($v,$f);
      else $f[$k]=$v;
    }
    return $f;
  }
}