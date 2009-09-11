(function ($) {
  Drupal.behaviors.hideButtons = {
    attach: function (context) {
      $("input.privatemsg-action-button").hide();
    }
  }
})(jQuery);