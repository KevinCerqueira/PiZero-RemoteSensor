<?php
include_once('Controller.php');

$controller = new Controller();
if(isset($_REQUEST['interval']) && !empty($_REQUEST['interval'])){	
	$response = $controller->sendInterval(intval($_REQUEST['interval']));
	echo json_encode($response);
	die();
}
echo json_encode(['success' => false, 'error' => 'Intervalo não informado.']);
die();