<?php

?>
<!DOCTYPE html>
<html lang="pt-br">

<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">

	<!-- bootstrap 5 -->
	<link href="vendor/bootstrap-5.2.0-beta1/dist/css/bootstrap.min.css" rel="stylesheet">
	<script src="vendor/bootstrap-5.2.0-beta1/dist/js/bootstrap.bundle.min.js"></script>

	<!-- jquery 3.6.0 -->
	<script src="vendor/jquery/jquery-3.6.0.min.js"></script>

	
	<style>
		.centro {
			width: 100%;
			height: 50px;
			line-height: 50px;
			text-align: center;
			background-color: rgba(10, 23, 55, 0.7);
			color: white;

			/* pura mágica */
			position: absolute;
			top: 50%;
			/* posiciona na metade da tela */
			margin-top: -25px;
			/* e retrocede metade da altura */
			/* margin: 0% 5% 0% 5%; */
		}

		.p-10 {
			padding: 10%;
		}
	</style>

	<title>Sensores de monitoramento</title>
</head>

<body>

	<div class="">
		<div class="centro text-center bg-white p-3 rounded shadow">
			<div class="row" id="list">
				<div class="col-md-4 p-0">
					<div id="high" class="lists m-1 text-white">

					</div>
				</div>
				<div class="col-md-4 p-0">
					<div id="medium" class="lists m-1 text-white">

					</div>
				</div>
				<div class="col-md-4 p-0">
					<div id="normal" class="lists m-1 text-white">

					</div>
				</div>
			</div>
		</div>
	</div>

	<script>
		// setInterval(requestData, 3000);
		// async function requestData() {
		// 	await $.ajax({
		// 		type: "GET",
		// 		url: "controllers/measures.php",
		// 		dataType: "html",
		// 		beforeSend: function() {
		// 			$('#alert-error').attr('hidden', '');
		// 			$('#alert-text-error').text('');

		// 			if ($('#count').text() == '0') {
		// 				$('#loading').removeAttr('hidden');
		// 			}
		// 		},
		// 		success: function(data) {
		// 			response = JSON.parse(data);
		// 			if (response.success) {
		// 				$('.lists').empty();

		// 				$("#high").append(response.data.high);
		// 				$("#medium").append(response.data.medium);
		// 				$("#normal").append(response.data.normal);
		// 				$('#count').text(parseInt($('#count').text()) + 1);
		// 				if (response.alert) {
		// 					console.log(sound.play());
		// 				}
		// 			} else {
		// 				$('#alert-error').removeAttr('hidden');
		// 				$('#alert-text-error').text('Não foi possível carregar a lista: ' + response.error);
		// 			}

		// 		},
		// 		error: function(data) {
		// 			console.log(data);
		// 			$('#alert-error').removeAttr('hidden');
		// 			$('#alert-text-error').text('Parece que estamos offline. Chame o TI!');
		// 		},
		// 		complete: function() {
		// 			$('#priorities').removeAttr('hidden');
		// 			$('#loading').attr('hidden', '');
		// 		}
		// 	});
		// }
	</script>

</body>

</html>