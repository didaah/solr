<?php
// $Id$
?>

<div id="solr_search_wrapper_<?php echo $group ?>">
  <?php echo $search->form?>
  <?php if ($search->is_result) : ?>
  <div class="solr_search_page_info">
    <?php if (!empty($search->sort)) : ?>
      <span class="solr_search_page_sort"><strong>排序：</strong><?php echo $search->sort?></span>
    <?php endif?>
    <span class="solr_search_page_result">找到相关记录 <?php echo $search->count?> 条，用时 <?php echo $search->time?> 秒</span>
  </div>
  <?php endif?>

  <div class="solr_search_page_content"><?php echo $search->content?></div>
  <?php echo $search->pager?>
</div>
