<?php

class Controller
{
	private $api;

	public function __construct()
	{
		// $this->setApi(true);
		// $this->api = "http://127.0.0.1:5000";
		$this->api = "http://".$_SERVER['REMOTE_ADDR'].":5000";
	}
	
	// public function setApi(bool $sum, string $addr = null)
	// {
	// 	if(!$addr){
	// 		return $this->api = "http://".$_SERVER['REMOTE_ADDR'].":5000";
	// 	}
	// 	$php_docker_ip = getHostByName(getHostName());
	// 	$last_num = substr($php_docker_ip, -1);
	// 	$addr =  substr($php_docker_ip,  0, strlen($php_docker_ip) - 1) . strval($sum ? ++$last_num : --$last_num);
	// 	return $this->api = "http://".$addr.":5000";
	// }

	public function getLast10Measures(): array
	{
		$response =  $this->api('GET', '/measures');
		// if(!$response['success']){
		// 	$this->setApi(false);
		// 	return $this->api('GET', '/measures');
		// }
		return $response;
	}
	
	public function sendInterval(int $interval): array
	{
		$response = $this->api('POST', "/interval/$interval");
		// if(!$response['success']){
		// 	$this->setApi(false);
		// 	return $this->api('POST', "/interval/$interval");
		// }
		return $response;
	}

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
			// var_dump($_SERVER['REMOTE_ADDR'], getHostByName(getHostName()));
// var_dump($response);die();
			// $response = json_decode($response);
			if (empty($response) || !$response) {
				return ['success' => false, 'error' => 'API não está respondendo.', 'trace' => 'URL ' . $url];
			}
			return ['success' => true, 'data' => json_decode($response)];
		} catch (Exception $e) {
			return ['sucess' => false, 'error' => $e->getMessage()];
		}
	}
}
