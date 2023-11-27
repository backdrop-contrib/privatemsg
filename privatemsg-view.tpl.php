<?php
print $anchors; ?>
<div <?php if ( !empty($message_classes)) { ?>class="<?php echo implode(' ', $message_classes); ?> container" <?php } ?> id="privatemsg-mid-<?php print $mid; ?>">
  <div class="row">
    <div class="privatemsg-author-avatar col-md-2">
      <?php print $author_picture; ?>
      <span class="privatemsg-author-name"><?php print $author_name_link; ?></span><br/>
      <span class="privatemsg-message-date"><?php print $message_timestamp; ?></span>
      <?php if (isset($new)): ?>
        <span class="new privatemsg-message-new"><?php print $new ?></span>
      <?php endif ?>
    </div>
    <div class="privatemsg-message-column col-md-10">
      <div class="privatemsg-message-information">
        <?php if (isset($message_actions)): ?>
          <?php print $message_actions ?>
        <?php endif ?>
      </div>
      <div class="privatemsg-message-body">
        <?php print $message_body; ?>
      </div>
    </div>
  </div>
</div>
