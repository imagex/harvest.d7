<?php

class harvest_plugin_query_harvest_entries extends harvest_plugin_query_harvest {
  function get_base_data() {
    return harvest_request("projects/{$node->harvest_project}/entries?from=19000101&to=" . date("Ymd"));
  }
}