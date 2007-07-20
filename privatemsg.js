// $Id$

if (Drupal.jsEnabled) {
  $(document).ready(
    function() {
      // Hide the elements we want hidden with JS.
      $('.js-hide').hide(0);

      // Show the elements we want to expose with JS.
      $('.js-show').show(0);

      // Add the JS to the delete button if necessary.
      $('.pm-add-delete').click(
        function() {
          var response = confirm(pm_delete_msg);
          if (response == true) {
            $('#edit-js-bypass').val(1);
          }
          return response;
        }
      );

      // Add the JS to the folder select if necessary.
      $('.pm-add-folder-select').change(
        function() {
          this.form.submit();
        }
      );
    }
  );
}
