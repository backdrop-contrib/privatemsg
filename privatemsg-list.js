(function ($) {
  Drupal.behaviors.hideButtons = {
    attach: function (context) {
      $("div.privatemsg-op-button").hide();

      $('#privatemsg-list #edit-operation').change(function () {
        $('#edit-submit').click();
      });
    }
  }
})(jQuery);