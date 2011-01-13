<?php
// $Id$
/**
 * @file
 * Administration functions for harvest projects.
 */

/**
 * Menu callback; form for managing project settings.
 */
function harvest_projects_settings_form() {
  $content_type_names = node_type_get_names();
  $form['harvest_projects_content_types_to_attach_to'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Attach to Content Types'),
    '#default_value' => variable_get('harvest_projects_content_types_to_attach_to', array('harvest_project')),
    '#options' => $content_type_names,
    '#description' => t('Choose which content types should have the option to have Harvest Project time reports associated with them. You must select at least one to make use of the Harvest Project module'),
  );
  $form['harvest_projects_non-billable_access'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Non-Billable Roles Access'),
    '#description' => t('Check the roles that will have access to non-billable hours reporting. If no roles are checked access will not be restricted.'),
    '#options' => user_roles(),
    '#default_value' => variable_get('harvest_projects_non-billable_access', array()),
  );
  return system_settings_form($form);
}

/**
 * Menu callback; form for managing task categories.
 */
function harvest_projects_categories_form() {
  $categories = harvest_projects_get_categories();
  $form['cid'] = array('#tree' => TRUE);

  if (count($categories)) {
    $form['cid'] = array('#tree' => TRUE);
    $form['name'] = array('#tree' => TRUE);
    $form['weight'] = array('#tree' => TRUE);

    // Loop through the categories and add form fields for it's elements.
    foreach ($categories as $cid => $category) {
      $form['cid'][$cid] = array(
        '#type' => 'value',
        '#value' => $category->cid,
      );
      $form['name'][$cid] = array(
        '#markup' => check_plain($category->name),
      );
      $form['description'][$cid] = array(
        '#markup' => check_plain($category->description),
      );
      $form['billable'][$cid] = array(
        '#markup' => check_plain($category->billable),
      );
      $form['weight'][$cid] = form_process_weight(array(
        '#delta' => count($categories) + 1,
        '#default_value' => $category->weight,
        '#attributes' => array('class' => array('category-weight')),
        '#parents' => array('weight', $cid),
      ));
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    drupal_set_message(t('There are no existing categories, you can now:') . ' ' . l(t('add one'), 'admin/harvest/categories/add'), 'warning');
  }

  return $form;
}

/**
 * Process the category management form, updating category weights.
 */
function harvest_projects_categories_form_submit(&$form, $form_state) {
  foreach ($form_state['values']['cid'] as $cid) {
    db_query("UPDATE {harvest_categories} SET weight = %d WHERE cid = %d", $form_state['values']['weight'][$cid], $cid);
  }
}

/**
 * Themes the task management form into a draggable table.
 */
function theme_harvest_projects_categories_form(&$variables) {
  $form = &$variables['form'];
  $output = '';
  // Build the table header.
  $header = array(t('Name'), t('Has Description'), t('Billable'), t('Operations'), t('Weight'));

  // Build the table rows from the categories.
  $rows = array();
  foreach (element_children($form['cid']) as $cid) {
    render($form['cid'][$cid]);
    $rows[] = array(
      'data' => array(
        render($form['name'][$cid]),
        drupal_strlen(trim(render($form['description'][$cid]))) ? t('Yes') : t('No'),
        render($form['billable'][$cid]) ? t('Yes') : t('No'),
        l(t('Edit'), "admin/harvest/categories/edit/$cid") . ', ' . l(t('Delete'), "admin/harvest/categories/delete/$cid"),
        render($form['weight'][$cid]),
      ),
      'class' => array('draggable'),
    );
  }

  // Build the final output of the form.
  if (count($rows)) {
    $output  = theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'harvest-categories'), 'colgroups' => array(), 'sticky' => FALSE, 'empty' => t('None'), 'caption' => NULL));
    drupal_add_tabledrag('harvest-categories', 'order', 'sibling', 'category-weight');
  }
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Menu callback; form for adding and editting categories.
 */
function harvest_projects_category_form($form, &$form_state, $cid = NULL) {
  $category = FALSE;

  if (is_numeric($cid)) {
    $category = harvest_projects_get_categories($cid);
    if (empty($category->name)) {
      drupal_set_message(t("Sorry, the category requested doesn't seem to exist."));
      drupal_goto('admin/harvest/categories');
    }
  }

  $form['cid'] = array(
    '#type' => 'hidden',
    '#value' => $cid,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Enter a display name for this category.'),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => $category ? $category->name : '',
  );
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#description' => t('Check the roles that can access this category. If no roles are checked access will not be restricted.'),
    '#options' => user_roles(),
    '#default_value' => ($category && is_array($category->roles)) ? $category->roles : array(),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Optionally enter a description for this category.'),
    '#default_value' => $category ? $category->description : '',
  );
  $form['billable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Is this a billable category?'),
    '#default_value' => ($category && isset($category->billable)) ? $category->billable : 1,
  );
  $form['weight'] = form_process_weight(array(
    '#type' => 'select',
    '#title' => t('Weight'),
    '#description' => t('Categories are displayed in ascending order by weight.'),
    '#delta' => count(harvest_projects_get_categories()) + 1,
    '#default_value' => $category ? $category->weight : '',
  ));
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save category'),
    '#suffix' => l(t('Cancel'), 'admin/harvest/categories'),
  );
  return $form;
}

/**
 * Processes the category form, updating existing entries or adding new entries.
 */
function harvest_projects_category_form_submit(&$form, $form_state) {
  harvest_projects_save_category($form_state['values']['cid'], $form_state['values']['name'], $form_state['values']['roles'], $form_state['values']['description'], $form_state['values']['billable'], $form_state['values']['weight']);
  $form['#redirect'] = 'admin/harvest/categories';
}

/**
 * Menu callback; form for deleting categories.
 */
function harvest_projects_delete_category_form($form_state, $cid) {
  $category = harvest_projects_get_categories($cid);
  if (empty($category->name)) {
    drupal_set_message(t("Sorry, the category requested doesn't seem to exist."));
    drupal_goto('admin/harvest/categories');
  }
  $form['cid'] = array(
    '#type' => 'hidden',
    '#value' => $cid,
  );
  return confirm_form($form, t("Are you sure you want to delete the '%category' category?", array('%category' => $category->name)), 'admin/harvest/categories', NULL, t('Delete'));
}

/**
 * Processes the category deletion form.
 */
function harvest_projects_delete_category_form_submit(&$form, $form_state) {
  harvest_projects_delete_category($form_state['values']['cid']);
  $form['#redirect'] = 'admin/harvest/categories';
}

/**
 * Menu callback; form for organizing project tasks.
 */
function harvest_projects_tasks_form($form, &$form_state, $node) {
  $tasks = harvest_projects_get_tasks($node->harvest_project);

  if (count($tasks)) {
    // Build a list of possible categories a task be under.
    $categories = harvest_projects_get_categories();
    $categories_options = array();
    foreach ($categories as $cid => $data) {
      $categories_options[$cid] = $data->name;
    }
    $categories_options['<none>'] = t('<none>');

    // Setup the trees for the various form items.
    $form['tid'] = array('#tree' => TRUE);
    $form['name'] = array('#tree' => TRUE);
    $form['category'] = array('#tree' => TRUE);
    $form['weight'] = array('#tree' => TRUE);

    foreach ($tasks as $cid => $data) {
      foreach ($data['tasks'] as $task_id => $task) {
        $form['tid'][$task_id] = array(
          '#markup' => check_plain($task_id),
        );
        $form['name'][$task_id] = array(
          '#markup' => check_plain($task['name']),
        );
        $form['category'][$task_id] = array(
          '#type' => 'select',
          '#options' => $categories_options,
          '#default_value' => $cid,
          '#required' => TRUE,
        );
        if (!$form['category'][$task_id]['#default_value']) {
          $form['category'][$task_id]['#default_value'] = '<none>';
        }
        $form['weight'][$task_id] = form_process_weight(array(
          '#delta' => count($data['tasks']) + 1,
          '#default_value' => 0,
          '#attributes' => array('class' => array('task-weight')),
          '#parents' => array('weight', $task_id),
        ));
      }
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    drupal_set_message(t('There are no existing tasks for this project, you can add them in harvest.'));
  }

  return $form;
}

/**
 * Processes the task organizational form.
 */
function harvest_projects_tasks_form_submit(&$form, $form_state) {
  drupal_set_message(t('All changes have been successfully saved.'));
  foreach ($form_state['values']['category'] as $tid => $cid) {
    $aid = harvest_projects_get_assignment(NULL, 'task-category', $tid, 'aid');
    if (is_numeric($cid) && $cid > 0) {
      harvest_projects_save_assignment($aid, 'task-category', $tid, $cid, $form_state['values']['weight'][$tid]);
    }
    elseif (!empty($aid)) {
      harvest_projects_delete_assignment($aid);
    }
  }
}

/**
 * Themes the task organization form.
 */
function theme_harvest_projects_tasks_form($variables) {
  $form = $variables['form'];
  $output = '';

  // Build the table header.
  $header = array(t('Name'), t('Category'), t('Weight'));

  // Make an array of categories to organize tasks by.
  $categories = harvest_projects_get_categories();
  $categorized_rows = array();
  foreach ($categories as $cid => $data) {
    $categorized_rows[$cid] = array();
  }
  $categorized_rows[0] = array();

  // Build the table rows from the categories.
  if (isset($form['tid'])) {
    $children = element_children($form['tid']);
  }
  if(isset($children)){
    foreach ($children as $task_id) {
      render($form['tid'][$task_id]);
      $categorized_rows[(is_numeric($form['category'][$task_id]['#default_value']) ? $form['category'][$task_id]['#default_value'] : 0)][] = array(
        'data' => array(
          array('data' => render($form['name'][$task_id]), 'class' => array('name-column')),
          array('data' => render($form['category'][$task_id]), 'class' => array('category-column')),
          render($form['weight'][$task_id]),
        ),
        'class' => array('draggable'),
      );
    }
  }
  

  // Put the categorized rows of tasks into actual table rows.
  foreach ($categorized_rows as $cid => $tasks) {
    $rows = array();
    if (count($tasks)) {
      foreach ($tasks as $task) {
        $rows[] = $task;
      }
      $output .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => "category-tasks-{$cid}"), 'caption' => ((isset($categories[$cid]) && $categories[$cid]->name) ? $categories[$cid]->name : t('Uncategorized')), 'colgroups' => array(), 'sticky' => FALSE, 'empty' => t('None')));
      drupal_add_tabledrag("category-tasks-{$cid}", 'order', 'sibling', 'task-weight');
    }
  }

  // Build the final output of the form.
  $output .= drupal_render_children($form);
  return $output;
}