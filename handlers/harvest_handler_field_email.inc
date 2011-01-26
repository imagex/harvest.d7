<?php

class harvest_handler_field_email extends harvest_handler_field {
  function render($values) {// dpm($values); dpm($this, '$this');
    $value = $values->{$this->field_alias};
    return html_entity_decode($value);
  }
}
