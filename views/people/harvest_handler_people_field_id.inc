<?php

class harvest_handler_people_field_id extends harvest_handler_people_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    return html_entity_decode($value);
  }
}
