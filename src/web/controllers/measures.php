<?php

/**
 * Componente Curricular: MI Concorrência e Conectividade
 * Autor: Esdras Abreu, Guilherme Nobre e Kevin Cerqueira
 *
 * Declaro que este código foi elaborado por mim de forma individual e
 * não contém nenhum trecho de código de outro colega ou de outro autor,
 * tais como provindos de livros e apostilas, e páginas ou documentos
 * eletrônicos da Internet. Qualquer trecho de código de outra autoria que
 * uma citação para o  não a minha está destacado com  autor e a fonte do
 * código, e estou ciente que estes trechos não serão considerados para fins
 * de avaliação. Alguns trechos do código podem coincidir com de outros
 * colegas pois estes foram discutidos em sessões tutorias.
 */

// Trata as medições para retornar para a view
include_once('Controller.php');

$controller = new Controller();
$response = $controller->getLast10Measures();
if ($response['success']) {
	$measures = "";
	if (isset($response['data']->measures)) {
		foreach ($response['data']->measures as $key => $medicao) {
			$datetime = date($medicao->datetime);
			$timestamp = strtotime($datetime);
			$date = date('d/m/Y H:i:s', $timestamp);
			$id = $key + 1;
			$measures .= "<a id='$medicao->id' class='list-group-item list-group-item-action'>";
			$measures .= "<div class='d-flex w-100 justify-content-between'>";
			$measures .=	"<span class='mb-1'>#$id</span>";
			$measures .=	"<div class='' style=''><ul class='p-2 list-group list-group-horizontal-md'> <li class='list-group-item'>Humidade: $medicao->H %</li> <li class='list-group-item'>Luminosidade: $medicao->L Lux</li> <li class='list-group-item'>Pressão: $medicao->P Pa</li> <li class='list-group-item'> Temperatura: $medicao->T °C</li></ul></div>";
			$measures .=	"<small class='text-muted'>$date</small>";
			$measures .= "</div>";
			$measures .= "</a>";
		}
	}
	echo json_encode(['success' => true, 'data' => ['measures' => $measures, 'interval' => $response['data']->interval]]);
	die();
}
echo json_encode($response);
die();
