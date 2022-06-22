<?php

class Controller
{
	private $api;

	public function __construct()
	{
		$this->api = 'http://10.0.0.109:5000';
	}

	public function getLast10Measures(): array
	{
		return $this->api('GET', $this->api . '/measures');
	}
	
	public function sendInterval(int $interval): array
	{
		return $this->api('POST', $this->api . "/interval/$interval");
	}

	function api(string $method, string $url, array $data = null): array
	{
		try {
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
			if (!$response) {
				return ['success' => false, 'message' => 'Houve uma falha na comunicação com a API.'];
			}
			return ['success' => true, 'data' => json_decode($response)];
		} catch (Exception $e) {
			return ['sucess' => false, 'message' => $e->getMessage()];
		}
	}
}
