<?php
include_once('Controller.php');

$controller = new Controller();
$response = $controller->getLast10Measures();
echo json_encode($response);
die();