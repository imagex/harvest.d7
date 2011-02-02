<?php

class harvest_handler_tasks_field extends views_handler_field {
  /**
   * Called to add the field to a query.
   */
  function query() {
    // $this->ensure_my_table();
    // Add the field.
    // $this->field_alias = $this->query->add_field($this->table_alias, $this->real_field);
    // $this->add_additional_fields();
    
    $this->field_alias = $this->real_field;
  }
}

class harvest_handler_tasks_field_id extends harvest_handler_tasks_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    return html_entity_decode($value);
  }
}

class harvest_handler_tasks_field_name extends harvest_handler_tasks_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    return html_entity_decode($value);
  }
}