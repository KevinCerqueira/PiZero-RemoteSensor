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

// Classe responsável por gerenciar as requisições a API
class Controller
{
	private $api;

	public function __construct()
	{
		// $this->api = "http://10.0.0.109:5000"; // Caso queira acessar de outros dispositivos da rede, será necessário colocar o IP atual do computador
		$this->api = "http://" . $_SERVER['REMOTE_ADDR'] . ":5000";
	}

	/**
	* Requisita a rota das 10 últimas medições
	*/
	public function getLast10Measures(): array
	{
		return $this->api('GET', '/measures');
	}

	/**
	* Envia o intervalo.
	*/
	public function sendInterval(int $interval): array
	{
		return $this->api('POST', "/interval/$interval");
	}
	
	/**
	* Prepara a requsição para ser feita a API
	*/
	function api(string $method, string $url, array $data = null): array
	{
		try {
			$url = $this->api . $url;
			$curl = curl_init($url);

			if ($method == 'POST') {
				curl_setopt($curl, CURLOPT_POST, 1);
				if ($data) {
					curl_setopt($curl, CURLOPT_POSTFIELDS, $data);
				}
			}

			curl_setopt($curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
			curl_setopt($curl, CURLOPT_USERPWD, "g02:thebestgroup");

			curl_setopt($curl, CURLOPT_URL, $url);
			curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

			$response = curl_exec($curl);
			curl_close($curl);
			if (empty($response) || !$response) {
				return ['success' => false, 'error' => 'API não está respondendo.', 'trace' => 'URL ' . $url];
			}
			return ['success' => true, 'data' => json_decode($response)];
		} catch (Exception $e) {
			return ['sucess' => false, 'error' => $e->getMessage()];
		}
	}
}
