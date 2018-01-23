#!/usr/bin/env php
<?php

require_once('./websockets.php');

class echoServer extends WebSocketServer {
  //protected $maxBufferSize = 1048576; //1MB... overkill for an echo server, but potentially plausible for other applications.

  protected function process ($user, $message) {
    $this->send($user,$message);
  }

  protected function connected ($user) {
    // Do nothing: This is just an echo server, there's no need to track the user.
    // However, if we did care about the users, we would probably have a cookie to
    // parse at this step, would be looking them up in permanent storage, etc.
    $gpio_on = shell_exec("/usr/local/bin/gpio -g write 26 0");
  }

  protected function closed ($user) {
    // Do nothing: This is where cleanup would go, in case the user had any sort of
    // open files or other objects associated with them.  This runs after the socket
    // has been closed, so there is no need to clean up the socket itself here.
    $gpio_on = shell_exec("/usr/local/bin/gpio -g write 27 0");
    $gpio_on = shell_exec("/usr/local/bin/gpio -g write 17 0");
    $gpio_on = shell_exec("/usr/local/bin/gpio -g write 4 0");
    $gpio_on = shell_exec("/usr/local/bin/gpio -g write 22 0");
  }
}

$echo = new echoServer("192.168.200.32","80");
$setmode17 = shell_exec("/usr/local/bin/gpio -g mode 17 out"); //UP
$setmode4  = shell_exec("/usr/local/bin/gpio -g mode 4 out"); //Down
$setmode22 = shell_exec("/usr/local/bin/gpio -g mode 22 out"); //Right
$setmode27 = shell_exec("/usr/local/bin/gpio -g mode 27 out"); //Left

try {
  $echo->run();
}
catch (Exception $e) {
  $echo->stdout($e->getMessage());
}
