<?php
// $Id$

if (empty($_args['group_type'])) {
  message('请提供分组名称');
  return;
}

if (!$info = solr_get_search_group($_args['group_type'])) {
  message('未定义的分组');
  return;
}

if (empty($info['fields'])) {
  message('该分组未定义任何字段，或均为动态字段，请自行更新 schema.xml');
  return;
}

$output = '';

foreach ($info['fields'] as $key => $v) {
  // @TODO 更多类型待完善
  $types = array(
    'string' => 'string',
    'int' => 'tlong',
    'float' => 'tfloat',
  );
  $output .= '<field name="' . $key . '" type="';
  $output .= !empty($types[$v['type']]) ? $types[$v['type']] : $v['type'];
  $output .= '" indexed="true" stored="true" ';
  $output .= 'multiValued="' . (!empty($v['multiple']) ? 'true' : 'false');
  $output .= '"/>';
  $output .= "\n";
}


echo "\n\n";
echo $output;
echo "\n";

