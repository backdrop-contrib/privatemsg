<?php
/**
 * @file
 * Class for private messages.
 */

/**
 * Defines the private messages class.
 */
class PrivatemsgMessage extends Entity {

  /**
   * The message ID.
   *
   * @var integer
   */
  public $mid;

  /**
   * The message type (bundle).
   *
   * @var string
   */
  public $type;


  /**
   * The uid of the user who is associated with the private message.
   *
   * @var integer
   */
  public $author;

  /**
   * The message subject.
   *
   * @var string
   */
  public $subject;

  /**
   * Constructor for private message entities.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values);
  }

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
    return isset($this->mid) ? $this->mid : NULL;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'privatemsg_message';
  }

  /**
   * Implements EntityInterface::bundle().
   */
  public function bundle() {
    return $this->entityType();
  }

  /**
   * Implements EntityInterface::label().
   */
  public function label() {
    return $this->subject;
  }

  /**
   * Implements EntityInterface::uri().
   */
  public function uri() {
    return array(
      'path' => 'messages/view/' . $this->mid,
      'options' => array(),
    );
  }
}
