<?php



/**
 * Message sending helper functions
 */
/**
 * Saves message to group association
 * 
 * group could be thread!
 *
 * @param $pmid - privatemsg id
 * @param $gid  - group id
 */
function _save_privatemsg_group($pmid, $gid) {
  //make sure that group and msg id are numeric
  if (!is_numeric($pmid) || !is_numeric($gid)) {
    return FALSE;
  }
  db_query("INSERT INTO {privatemsg_group} (pmid, gid) VALUES (%d, %d)", $pmid, $gid);
}

/**
 * Save group to users association
 * 
 * This message is called only when a new message is being sent
 * 'new message' or 'forward message' are qualified new messages for which group_user must be created
 *
 * @param $gid - group id
 * @param $users - array of user ID
 */
function _save_privatemsg_group_user($gid, $users = array()) {
  //make sure all conditions are met for this function to work
  if (!is_numeric($gid) || !is_array($user) || empty($user)) {
    return FALSE;
  }
  //build the query, regardless of how many users added, this will execute only one query
  $query = "INSERT INTO {privatemsg_group_user} (gid, uid) VALUES ";
  $values = array();
  $replacement = array();
  foreach ( $users as $index => $user ) {
    $values[]      = "(%d, %d)";
    $replacement[] = $gid;
    $replacement[] = $uid;
  }
  
  $query .= implode(", ", $values);
  db_query($query, $replacement);
}

/**
 * Privatemsg load single msg api 
 *
 */
function _privatemsg_load($pmid, $uid = NULL) {
  $query = _privatemsg_assemble_query('privatemsg_load', $pmid, $uid);
//  drupal_set_message('<pre>'. print_r($query, 1) . '</pre>');

  $result = db_query($query);
  $message = db_fetch_array($result);
  return $message;
}

function _privatemsg_assemble_query($query_id) {
  
  $SELECT = array();
  $INNER_JOIN = array();
  $WHERE = array();
  $GROUP_BY = array();
  $ORDER_BY = array();
  $QUERY_ARGS = array();
  $primary_table = '';
  
  $fragments = array(
    'select'      => $SELECT,
    'inner_join'  => $INNER_JOIN,
    'where'       => $WHERE,
    'group_by'    => $GROUP_BY,
    'order_by'    => $ORDER_BY,
    'query_args'  => $QUERY_ARGS,
    'primary_table'  => $primary_table,
  );
  
  /**
   * Begin: dynamic arguments
   */
  $args = func_get_args();
  unset($args[0]);
  //we do the merge because we call call_user_func_array and not drupal_alter
  //this is necessary because otherwise we would not be able to use $args correctly (otherwise it doesnt unfold)
  if (!empty($args)) {
     $alterargs = array_merge(array($query_id, &$fragments,), $args);
  }
  else {
     $alterargs = array($query_id, &$fragments,);
  }
  /**
   * END: Dynamic arguments  
   */
  call_user_func_array('drupal_alter', $alterargs);
  
  $SELECT = $fragments['select'];
  $INNER_JOIN = $fragments['inner_join'];
  $WHERE = $fragments['where'];
  $GROUP_BY = $fragments['group_by'];
  $ORDER_BY = $fragments['order_by'];
  $QUERY_ARGS = $fragments['query_args'];
  $primary_table = $fragments['primary_table'];
  
  if(empty($primary_table)) {
    $primary_table = '{privatemsg} pm';
  }
  //perform the whole query assembly only if we have something to select
  if (!empty($SELECT)) {
    $str_select = implode(", ", $SELECT);
    $query = "SELECT {$str_select} FROM ". $primary_table;
    
    if (!empty($INNER_JOIN)) {
      $str_inner_join = implode(' ', $INNER_JOIN);
      $query .= " {$str_inner_join}";
    }
    if (!empty($WHERE)) {
      $str_where = '('. implode(') AND (', $WHERE) .')';
      $query .= " WHERE {$str_where}";
    }
    if (!empty($GROUP_BY)) {
      $str_group_by = ' GROUP BY '. implode(", ", $GROUP_BY) ;
      $query .= " {$str_group_by}";
    }
    if (!empty($ORDER_BY)) {
      $str_order_by = ' ORDER BY '. implode(", ", $ORDER_BY) ;
      $query .= " {$str_order_by}";
    }
    $return['query'] = $query;
    if (!empty($QUERY_ARGS)) {
      $query = vsprintf($query, $QUERY_ARGS);
    }
    return $query;
  }
  return FALSE;
}