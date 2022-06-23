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

	<title>Monitoramento</title>
</head>

<body class="bg-dark">

	<div class="p-5">
		<div class="list-group">
			<a class="list-group-item list-group-item-action active" aria-current="true">
				<div class="text-center">
					<h5 class="mb-1">Lista das 10 últimas medições</h5>
					<small class="text-white">Refresh: #</i><span id="count">0</span></small>
					<div hidden id="alert-error" class="alert alert-danger mt-3" role="alert">
						<p id="alert-text-error" class="h5 m-0"></p>
					</div>
				</div>
			</a>
			<div id="medicoes">
				<div id="loading">
					<a href="#" class="list-group-item list-group-item-action">
						<div class="d-flex w-100 justify-content-between">
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
							<div class="spinner-grow text-dark" role="status">
								<span class="visually-hidden">Carregando...</span>
							</div>
						</div>
					</a>
				</div>
				<div class="lists">

				</div>
			</div>
		</div>
	</div>

	<script>
		$(function(event) {
					setInterval(requestData, 1000);
					async function requestData() {
						await $.ajax({
							type: "GET",
							url: "controllers/measures.php",
							dataType: "html",
							beforeSend: function() {
								$('#alert-error').attr('hidden', '');
								$('#alert-text-error').text('');

								if ($('#count').text() == '0') {
									$('#loading').removeAttr('hidden');
								}
							},
							success: function(data) {
								response = JSON.parse(data);
								if (response.success) {
									$('.lists').empty();

									$(".lists").append(response.data);

									$('#count').text(parseInt($('#count').text()) + 1);
								} else {
									$('#alert-error').removeAttr('hidden');
									$('#alert-text-error').text('Não foi possível carregar a lista: ' + response.error);
								}

							},
							error: function(data) {
								console.log(data);
								$('#alert-error').removeAttr('hidden');
								$('#alert-text-error').text('Parece que estamos offline. Chame o TI!');
							},
							complete: function() {
								$('#priorities').removeAttr('hidden');
								$('#loading').attr('hidden', '');
							}
						});
					}
				});
	</script>

</body>

</html>