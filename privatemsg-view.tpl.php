<?php
// $Id$
// Each file loads it's own styles because we cant predict which file will be
// loaded.
drupal_add_css(drupal_get_path('module', 'privatemsg') . '/styles/privatemsg-view.css');
?>
<?php print $anchors; ?>
<div class="privatemsg-message" id="privatemsg-mid-<?php print $mid; ?>">
  <div class="privatemsg-author-column">
    <div class="privatemsg-author-column-left">
      <div class="privatemsg-author-avatar">
        <?php print $author_picture; ?>
      </div>
    </div>
  </div>
  <div class="privatemsg-author-column-right">
    <div class="privatemsg-author-name">
      <?php print $author_name_link; ?>
    </div>
    <div class="privatemsg-message-date">
      <?php print $message_timestamp; ?>
    </div>
  </div>
  <div class="privatemsg-message-column">
    <div class="privatemsg-message-body">
      <?php if (isset($new)) : ?>
        <span class="new"><?php print $new ?></span>
      <?php endif ?>
      <?php print $message_body; ?>
    </div>
    <?php if ( isset($message_actions)) : ?>
       <?php print $message_actions ?>
    <?php endif ?>
  </div>
  <div class="clear-both privatemsg-bottom-border"></div>
</div>
