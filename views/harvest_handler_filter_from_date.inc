<?php

class harvest_handler_filter_from_date extends harvest_handler_filter_date {
  function query() {
    $this->query->add_where($this->options['group'], "")
  }
}