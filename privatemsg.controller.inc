<?php

/**
 * Private message controller, loads private messages.
 */
class PrivatemsgMessageController extends DefaultEntityController {

  protected $account = NULL;

  protected function attachLoad(&$messages, $revision_id = FALSE) {
    global $user;
    foreach ($messages as $message) {
      $message->user = $this->account ? $this->account : $user;
      // Load author of message.
      if (!($message->author = user_load($message->author))) {
        // If user does not exist, load anonymous user.
        $message->author = user_load(0);
      }
    }
    parent::attachLoad($messages, $revision_id);
  }

  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    // Remove account from conditions.
    if (isset($conditions['account'])) {
      $this->account = $conditions['account'];
      unset($conditions['account']);
    }

    $query = parent::buildQuery($ids, $conditions, $revision_id);
    $query->fields('pmi', array('is_new', 'thread_id'));
    if ($this->account) {
      $query
        ->condition('pmi.recipient', $this->account->uid)
        ->condition('pmi.type', array('hidden', 'user'));
    }
    else {
      // If no account is given, at least limit the result to a single row per
      // message.
      $query->distinct();
    }
    $query->join('pm_index', 'pmi', "base.mid = pmi.mid");
    return $query;
  }

  /**
   * {@inheritdoc}
   */
  protected function cacheGet($ids, $conditions = array()) {
    // Passing the account condition, which does not exist as a property to
    // parent::cacheGet() causes notices, remove it.
    // @todo Investigate if this causes any undesired side effects.
    if (isset($conditions['account'])) {
      unset($conditions['account']);
    }
    return parent::cacheGet($ids, $conditions);
  }
}
