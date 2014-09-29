<?php
// $Id$

/**
 * 搜索页面
 * @param (string) $group
 *  分组名称，若未指定，则为默认搜索
 */
function solr_page_search($group = NULL) {
  if (!$groups = solr_get_search_group()) {
    // 没有任何搜索分组，无法提供搜索服务
    dd_get_access();
  }
  
  foreach ($groups as $key => $item) {
    if (user_access('solr search ' . $key . ' view') && var_get_key('solr', 'service_' . $key)) {
      $tab_title = empty($item['page_title']) ? $item['title'] : $item['page_title'];
      $tabs[] = l($tab_title, 'search/' . $key);
      if (!isset($group)) {
        $group = $key;
      }
    }
  }
  
  if (empty($group)) {
    // 没有权限访问任何搜索服务
    dd_get_access();
  }

  $search = new stdClass; 
  $search->form = dd_get_form('solr_search_form', $group);
  $search->is_result = false;
  
  if (!empty($_GET['op']) && $_GET['op'] == 'search') {
    $solr = new solr($group);
    
    $search->is_result = true;
    
    $args = $_GET;
    
    if (isset($args['query'])) {
      $args['query'] = urldecode(trim($args['query']));
      $page_title[] = $args['query'];
      $args['q'] = $args['query'];
      // 将中文查询短语分词
      if (!empty($groups[$group]['query_keywords'])) {
        $args['q'] = preg_replace_callback("/([\x{4e00}-\x{9fa5}]+)/u", 'solr_get_cn_keywords', $args['q']);
      }
      unset($args['query']);
    }
    
    $search->count = $search->time = 0;
    
    module_alter_all('solr_search_query', $group, $args);
    
    if ($result = $solr->query($args)) {
      
      if (isset($result->responseHeader->QTime)) {
        $search->time = $result->responseHeader->QTime/1000;
      }

      // 提交至 solr 的查询参数
      $search->query = $solr->queryData; 

      // solr 返回的查询参数
      $search->params = $result->responseHeader->params;

      if (!empty($result->response->docs)) {
        $search->sort = module_invoke($groups[$group]['module'], 'solr_search_sort', $group);
        $search->count = $result->response->numFound;
        $search->content = module_invoke($groups[$group]['module'], 'solr_search_view', $group, $result);
        pager_lists($result->response->numFound, $result->responseHeader->params->rows);
        $search->pager = pager_view(array('no_input' => true));
      } else {
        if (!$search->content = module_invoke($groups[$group]['module'], 'solr_search_no_result', $group, $result)) {
          $search->content = system_no_content(t('solr', '没有匹配的记录，换个关键词试试'));
        }
      }
    }
    
  } else {
    $search->content = module_invoke($groups[$group]['module'], 'solr_search_default_view', $group);
  }
  
  $page_title[] = empty($groups[$group]['page_title']) ? $groups[$group]['title'] : $groups[$group]['page_title'];
  $page_title[] = t('solr', '搜索');
  
  dd_set_title($page_title);
  dd_set_tabs($tabs);
  
  dd_add_css(dd_get_path('module', 'solr') . '/solr.css');
  
  return theme('solr_search', $search, $group);
}
