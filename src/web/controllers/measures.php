<?php
include_once('Controller.php');

$controller = new Controller();
$response = $controller->getLast10Measures();
if($response['success']){
	$measures = "";
	foreach($response['data']->measures as $key => $medicao){
		$datetime = date($medicao->datetime);
		$timestamp = strtotime($datetime);
		$date = date('d/m/Y H:i:s', $timestamp); 
		$id = $key + 1;
		$measures .= "<a id='$medicao->id' class='list-group-item list-group-item-action'>";
		$measures .= "<div class='d-flex w-100 justify-content-between'>";
		$measures .=	"<span class='mb-1'>#$id</span>";
		$measures .=	"<div class='' style=''><ul class='p-2 list-group list-group-horizontal-md'> <li class='list-group-item'>Humidade: $medicao->H</li> <li class='list-group-item'>Luminosidade: $medicao->L</li> <li class='list-group-item'>Potenciometro: $medicao->P </li> <li class='list-group-item'> Temperatura: $medicao->T </li></ul></div>";		
		// $measures .=	"<ul class='list-group list-group-horizontal'> <li class='list-group-item list-group-item-info'>Humidade: $medicao->H</li> <li class='list-group-item list-group-item-info'>Luminosidade: $medicao->L</li> <li class='list-group-item list-group-item-info'>Potenciometro: $medicao->P </li> <li class='list-group-item list-group-item-info'> Temperatura: $medicao->T </li></ul>";		
		$measures .=	"<small class='text-muted'>$date</small>";
		$measures .= "</div>";
		// $measures .=	"<p class='mb-1 row'> <span class='col-md-3'>Humidade: $medicao->H</span> <span class='col-md-3'>Luminosidade: $medicao->L</span> <span class='col-md-3'>Potenciometro: $medicao->P </span> <span class='col-md-3'> Temperatura: $medicao->T </span></p>";		
		$measures .= "</a>";
	}
	echo json_encode(['success' => true, 'data' => ['measures' => $measures, 'interval' => $response['data']->interval]]);
	die();
}
echo json_encode($response);
die();