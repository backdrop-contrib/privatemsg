function privatemsgContactFill() {
  $('#edit-quick').css('display', 'block');
  $('#edit-quick').change( function() {
    $('#edit-recipient').val(this.options[this.selectedIndex].value);
    this.selectedIndex = 0;
  });
}

if (Drupal.jsEnabled) {
  $(document).ready(privatemsgContactFill);
}
