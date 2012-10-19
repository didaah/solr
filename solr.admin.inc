<?php
// $Id$

/**
 * solr 设置
 */
function solr_admin_settings() {
  dd_set_tabs(array(
    l(t('solr', 'solr 设置'), 'admin/solr'),
    l(t('solr', '索引任务'), 'admin/solr/index'),
  ));

  if (!$groups = solr_get_search_group()) {
    dd_set_message(t('solr', '还没有定义搜索类型'));
    return;
  }

  $i = false;

  foreach ($groups as $key => $value) {
    $options[$key] = $value['title'];
    $form['fields']['solr']['service_' . $key] = array(
      '#title' => $value['title'],
      '#type' => 'textfield',
      '#required' => 1,
      '#default_value' => var_get_key('solr', 'service_' . $key, $value['service']),
    );
    if ($i === false) {
      $form['fields']['solr']['service_' . $key]['#fieldset'] = 1;
      $form['fields']['solr']['service_' . $key]['#fieldset_prefix'] = 'asc';
      $form['fields']['solr']['service_' . $key]['#fieldset_legend'] = '服务地址';
      $form['fields']['solr']['service_' . $key]['#description'] = '如： http://localhost:8983/solr';
    }
    $i = $key;
  }
  
  $form['fields']['solr']['service_' . $i]['#fieldset_suffix'] = 1;
  $form['fields']['solr']['service_' . $i]['#fieldset'] = 1;
  
  $form['fields']['solr']['keywords_service'] = array(
    '#title' => '分词地址',
    '#type' => 'textfield',
    '#default_value' => var_get_key('solr', 'keywords_service'),
    '#description' => '用做查询预处理，如：http://localhost:8983/solr/analysis/field'
  );
  
  $form['fields']['solr']['default_search'] = array(
    '#type' => 'select',
    '#title' => t('solr', '默认搜索'),
    '#required' => 1,
    '#default_value' => var_get_key('solr', 'default_search', 'user'),
    '#options' => $options
  );
  
  return system_setting_form($form);
}

/**
 * 索引任务
 */
function solr_admin_index() {
  
  dd_set_tabs(array(
    l(t('solr', 'solr 设置'), 'admin/solr'),
    l(t('solr', '索引任务'), 'admin/solr/index'),
  ));
 
  if (!$groups = solr_get_search_group()) {
    dd_set_message(t('solr', '还没有定义搜索类型'));
    return;
  }
  
  if (arg(3) && $groups[arg(3)]) {
    
    if ($_GET['op'] == 'import') {
      // 批量导入索引
      solr_data_import(0, arg(3));
      dd_goto('admin/solr/index');
    } else if ($_GET['op'] == 'clear') {
      
      if (solr_data_clear(arg(3))) {
        echo t('solr', '清除成功，您可能需要重建索引');
      }
      
      exit;
    }
    
  }
  
  foreach ($groups as $key => $value) {
    $item = array();
    
    $item[] = l('导入数据', 'admin/solr/index/' . $key, array('query' => 'op=import'));
    
    $item[] = l('清空索引', 'admin/solr/index/' . $key,
      array('query' => 'op=clear', 'attributes' => array('class' => 'confirmajax')));
    
    $output .= theme('item_list', $item, $value['title']);
  }
  
  return $output;
}
