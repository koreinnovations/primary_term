<?php

function primary_term_action_info() {
  if (!module_exists('taxonomy'))
    return array();
  return array('primary_term_action' => array(
          'type' => 'node',
          'description' => t('Modify node taxonomy terms'),
          'configurable' => TRUE,
          'behavior' => array('changes_node_property'),
  ));
}

function primary_term_action(&$node, $context) {
  /*
  // Delete current setting of primary term
  db_query('DELETE FROM {primary_term} WHERE vid = %d', $node->vid); // only one term per node revision

  if ($context['tid']) {
    db_query('INSERT INTO {primary_term} (vid, tid) VALUES (%d, %d)', $node->vid, $context['tid']);
  }
  */
  $node->primaryterm = $context['tid'];
}

function primary_term_action_form($context) {
  if (isset($context['selection']) && isset($context['view'])) {
    $vocabularies = array();
    $nids = array_map('_views_bulk_operations_get_oid', $context['selection'], array_fill(0, count($context['selection']), $context['view']->base_field));
    $result = db_query("SELECT DISTINCT v.vid FROM {vocabulary_node_types} v LEFT JOIN {node} n ON v.type = n.type WHERE n.nid IN (%s)", implode(',', $nids));
    while ($v = db_fetch_object($result)) {
      $vocabularies[$v->vid] = taxonomy_vocabulary_load($v->vid);
    }
  }
  else {
    $vocabularies = taxonomy_get_vocabularies();
  }
  if (empty($vocabularies)) {
    drupal_set_message(t('The selected nodes are not associated with any vocabulary. Please select other nodes and try again.'), 'error');
    return array();
  }
  $form['taxonomy'] = array(
      '#type' => 'fieldset',
      '#title' => t('Vocabularies'),
      '#tree' => TRUE,
  );
  while (list(, $vocabulary) = each($vocabularies)) {
    $form['taxonomy'][$vocabulary->vid] = taxonomy_form($vocabulary->vid, isset($context['terms']) ? $context['terms'] : NULL);
    $form['taxonomy'][$vocabulary->vid]['#weight'] = $vocabulary->weight;
    if ($vocabulary->tags) { // If tags, give the ability to add new terms
      if ($vocabulary->help) {
        $help = $vocabulary->help;
      }
      else {
        $help = t('A comma-separated list of terms describing this content. Example: funny, bungee jumping, "Company, Inc.".');
      }
      $help .= t('<br />Note that this field has no effect when deleting terms.');
      $form['taxonomy']['tags'][$vocabulary->vid] = array('#type' => 'textfield',
          '#title' => $vocabulary->name . ' ' . t('(new tags)'),
          '#autocomplete_path' => 'taxonomy/autocomplete/' . $vocabulary->vid,
          '#weight' => $vocabulary->weight,
          '#maxlength' => 1024,
          '#description' => $help,
          '#default_value' => isset($context['terms']['tags'][$vocabulary->vid]) ? $context['terms']['tags'][$vocabulary->vid] : '',
      );
    }
  }
  $form['do'] = array(
      '#type' => 'radios',
      '#title' => t('Action to take'),
      '#default_value' => isset($context['do']) ? $context['do'] : TAXONOMY_ACTION_ADD,
      '#options' => array(
          TAXONOMY_ACTION_ADD => t('Add the selected terms'),
          TAXONOMY_ACTION_REPLACE => t('Replace existing terms with selected ones'),
          TAXONOMY_ACTION_REPLACE_VOCABULARY => t('Replace terms within same vocabulary'),
          TAXONOMY_ACTION_DELETE => t('Delete selected terms')
      ),
      '#required' => TRUE,
      '#weight' => -2,
  );
  return $form;
}

function primary_term_action_submit($form, $form_state) {
  return array(
      'do' => $form_state['values']['do'],
      'term' => $form_state['values']['tid'],
  );
}
