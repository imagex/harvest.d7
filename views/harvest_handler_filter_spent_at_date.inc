<?php

class harvest_handler_filter_spent_at_date extends harvest_handler_filter_date {
  function query() {    
    $this->value['value'] = format_date(strtotime($this->value['value']), 'custom', 'Ymd');
    
    switch ($this->operator) {
      case '>=':
        $this->query->add_where($this->options['group'], 'from=' . $this->value['value']);
        break;
      case '<=':
        $this->query->add_where($this->options['group'], 'to=' . $this->value['value']);
        break;
    }
  }
  
  /**
   * Render our chunk of the exposed filter form when selecting
   *
   * You can override this if it doesn't do what you expect.
   */
  function exposed_form(&$form, &$form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }

    // Build the exposed form, when its based on an operator.
    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator'])) {
      $operator = $this->options['expose']['operator'];
      $this->operator_form($form, $form_state);
      $form[$operator] = $form['operator'];

      if (isset($form[$operator]['#title'])) {
        unset($form[$operator]['#title']);
      }

      $this->exposed_translate($form[$operator], 'operator');

      unset($form['operator']);
    }

    // Build the form and set the value based on the identifier.
    if (!empty($this->options['expose']['identifier'])) {
      $value = $this->options['expose']['identifier'];
      $this->value_form($form, $form_state);
      $form[$value] = $form['value'];

      if (isset($form[$value]['#title']) && !empty($form[$value]['#type']) && $form[$value]['#type'] != 'checkbox') {
        unset($form[$value]['#title']);
      }

      $this->exposed_translate($form[$value], 'value');

      if (!empty($form['#type']) && ($form['#type'] == 'checkboxes' || ($form['#type'] == 'select' && !empty($form['#multiple'])))) {
        unset($form[$value]['#default_value']);
      }

      if (!empty($form['#type']) && $form['#type'] == 'select' && empty($form['#multiple'])) {
        $form[$value]['#default_value'] = 'All';
      }

      if ($value != 'value') {
        unset($form['value']);
      }
      
      // We need to set a class so we can attach datepicker()
      $form[$value]['#attributes'] = array('class' => array('spent-at-date'));
      
      // If isn't loaded, load jQuery UI's Datepicker libs with this form
      if (!isset($form['#attached']['library']) || !in_array(array('system', 'ui.datepicker'), $form['#attached']['library'])) {
        $form['#attached']['library'][] = array('system', 'ui.datepicker');
      }
      
      // Now we will try to assign datepicker to our field
      if (!isset($form['#attached']['js'])) {
        $form['#attached']['js'] = array(
          drupal_get_path('module', 'harvest') . '/js/views.js' => array(
            'type' => 'file',
          ),
        );
      }
    }
  }
}