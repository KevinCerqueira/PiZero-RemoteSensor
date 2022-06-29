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

include_once('Controller.php');

$controller = new Controller();
if(isset($_REQUEST['interval']) && !empty($_REQUEST['interval'])){	
	$response = $controller->sendInterval(intval($_REQUEST['interval']));
	echo json_encode($response);
	die();
}
echo json_encode(['success' => false, 'error' => 'Intervalo não informado.']);
die();