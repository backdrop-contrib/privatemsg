if (isJsEnabled()) {
  addLoadEvent(privatemsgContactFill);
}

function privatemsgContactFill() {
  if ($('edit-quick')) {
    $('edit-quick').style.display = '';
    $('edit-quick').onchange = function() {
      $('edit-recipient').value = $('edit-quick').value;
    };
  }
}
