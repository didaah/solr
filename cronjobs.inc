<?php
// $Id$

/**
 * @file 自定义计划任务
 */

/**
 * @Implement of hook_cronjobs()
 * @param array $timestamp
 *  所定义的每个任务上次执行时间
 */
function solr_cronjobs($timestamp) {
  
  $cronjobs = array();
  
  $cronjobs['get_fields'] = array(
    'name' => t('solr', '获取字段'),
    'description' => t('solr', '获取应用于 solr schema.xml 的字段'),
    'script' => 'get_fields.inc',
    'year' => '3055',
    'month' => '*',
    'day' => '*',
    'hour' => '*',
    'minute' => '1', 
  );

  $cronjobs['command'] = array(
    'name' => t('solr', '运行命令'),
    'description' => t('solr', '用于向 solr 发送请求，不自动运行，根据需要自行执行'),
    'script' => 'command.inc',
    'year' => '3055',
    'month' => '*',
    'day' => '*', 
    'hour' => '*',
    'minute' => '50', 
  );

  $cronjobs['update'] = array(
    'name' => t('solr', '索引更新'),
    'description' => t('solr', '每 10 分钟自动更新'),
    'param' => array('last_timestamp' => $timestamp),
    'script' => 'update.inc',
    'year' => '*',
    'month' => '*',
    'day' => '*', 
    'hour' => '*',
    'minute' => '*/10', 
  );

  return $cronjobs;
}
