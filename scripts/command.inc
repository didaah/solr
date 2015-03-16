<?php
// $Id$

if (empty($_args['group_type'])) {
  message('请提供分组名称');
  return;
}

$group_type = $_args['group_type'];

if (empty($_args['solr_command'])) {
  message('请提供要执行的命令，如：update、delete、commit 等');
  return;
}

if (!$info = solr_get_search_group($group_type)) {
  message('未定义的分组');
  return;
}

$commit = !empty($_args['is_commit']) ? true : false;

switch ($_args['solr_command']) {
  case 'update':
    // 更新数据
    $last_timestamp = var_get('cron_last_time', 0);
    solr_data_import($last_timestamp, $group_type, $commit);
  break;
  case 'commit':
    // 使索引生效
    $solr = new solr($group_type);
    if ($result = $solr->commit()) {
      message($result);
    } else {
      message($solr->error);
    }
  break;
  case 'optimize':
    // 优化索引
    $solr = new solr($group_type);
    if ($result = $solr->optimize()) {
      message($result);
    } else {
      message($solr->error);
    }
  break;
  case 'delete_all':
    // 删除所有
    solr_data_clear($group_type);
  break;
  case 'delete_query':
    // 删除匹配的记录
    if (!empty($_args['solr_query'])) {
      $solr = new solr($group_type);
      if ($result = $solr->deleteQuery($_args['solr_query'])) {
        message($result);
      } else {
        message($solr->error);
      }
    }
  break;
  case 'rollback':
    // 回滚
    $solr = new solr($group_type);
    if ($result = $solr->rollback()) {
      message($result);
    } else {
      message($solr->error);
    }
}

